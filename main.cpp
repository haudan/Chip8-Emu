//
//  main.cpp
//  Chip-8 Emu
//
//  Created by Daniel Hauser on 14.12.14.
//  Copyright (c) 2014 Daniel Hauser. All rights reserved.
//

#include <cstdio>
#include <memory>
#include <vector>
#include <thread>

#include <SDL2/SDL.h>

#include "Emulator.hpp"
#include "Screen.hpp"
#include "Random.hpp"

void Sleep(int ms)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void LogEmu(C8::Emulator &emu)
{
  // Generic registers
  for(int i = 0; i < 16; ++i)
  {
    std::printf("%2X ", emu.mRegV[i]);
  }
  
  std::puts("");
  
  for(int i = 0; i < 16; ++i)
  {
    std::printf("V%X ", i);
  }
  
  std::puts("\n");
  
  // Main registers
  std::printf("%3X  %3X  %3X  %3X  %3X\n", emu.mRegI, emu.mRegPC, emu.mRegSP, emu.mRegDT, emu.mRegST);
  std::printf("  I   PC   SP   DT   ST\n");
  
  std::puts("\n");
}

/*uint8_t KeyCode(int key)
{
  return (key >= SDLK_0 && key <= SDLK_9) ? key - SDLK_0
  : (key >= SDLK_a && key <= SDLK_f) ? key - SDLK_a + 10
  : 0xFF;
}*/

// This version expects the passed key to actually be the scancode.
uint8_t KeyCode(int key)
{
#define kc(c) SDL_SCANCODE_##c
#define is(c) (key == kc(c))
  
  return (key >= kc(1) && key <= kc(3)) ? key - kc(1) + 1 : is(4) ? 0xC
  : is(Q) ? 0x4 : is(W) ? 0x5 : is(E) ? 0x6 : is(R) ? 0xD
  : is(A) ? 0x7 : is(S) ? 0x8 : is(D) ? 0x9 : is(F) ? 0xE
  : is(Z) ? 0xA : is(X) ? 0x0 : is(C) ? 0xB : is(V) ? 0xF
  : 0xFF;
  
#undef is
#undef kc
}

void AudioCallback(void *userData, Uint8 *data, int len)
{
  float *stream = (float*)data;
  const size_t length = len / sizeof(float);
  C8::Emulator *emu = (C8::Emulator*)userData;
  
  const bool playSound = emu->ShouldPlaySound();
  
  constexpr float MaxVolume = 0.25f;
  constexpr float Volume = 1.0f;
  constexpr float Delta = 1.0f / (44100.0f / 4096.0f);
  constexpr float Pi = 3.14159265359f;
  constexpr float Freq = Pi / 4;
  
  static float rot = 0;
  
  for(size_t i = 0; i < length; ++i)
  {
    if(playSound)
    {
      float val = std::sin(rot);
      rot += Freq * Delta;
      rot += (Pi / 256) * RandReal<float>(-1.0f, 1.0f);
      // Yes this is all done on purpose.
      
      if(rot > 2 * Pi) rot -= 2 * Pi;
      
      stream[i] = val * Volume * MaxVolume;
    }
    else
    {
      stream[i] = 0;
    }
  }
}

int main(int argc, char **argv)
{
  if(argc < 2)
  {
    std::puts("No file specified.");
    return 0;
  }
  
  FILE *fp = std::fopen(argv[1], "rb");
  if(!fp)
  {
    std::printf("'%s' not found.", argv[1]);
    return 0;
  }
  
  fseek(fp, 0, SEEK_END);
  auto fileSize = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  
  std::unique_ptr<uint8_t[]> program(new uint8_t[fileSize]);
  fread(program.get(), 1, fileSize, fp);
  fclose(fp);
  
  bool programRunning = true;
  
  // Init SDL
  
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  UI::Screen screen{"CHIP-8 Emulator", 960, 480,
    C8::Emulator::VideoWidth, C8::Emulator::VideoHeight};
  
  C8::Emulator emu;
  
  // Audio
  
  bool hasAudio = false;
  
  SDL_AudioSpec have = {0}, want =
  {
    .freq = 44100,
    .format = AUDIO_F32,
    .channels = 1,
    .samples = 4096,
    .callback = &AudioCallback,
    .userdata = &emu
  };
  
  SDL_AudioDeviceID audioDevice = SDL_OpenAudioDevice(nullptr, 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
  if(audioDevice == 0)
  {
    SDL_ShowSimpleMessageBox(0, "Emulator Error", "Couldn't open audio device!", nullptr);
  }
  else
  {
    if(have.format != want.format)
    {
      SDL_ShowSimpleMessageBox(0, "Emulator Error", "Couldn't open audio device with Float32 format!", nullptr);
    }
    else hasAudio = true;
  }
  
  if(hasAudio)
  {
    SDL_PauseAudioDevice(audioDevice, 0);
    std::puts("Audio ready");
  }
  
  // Emulator
  
  emu.LoadProgram(program.get(), fileSize);
  emu.Start();
  
  while(emu.Running() && programRunning)
  {
    SDL_Event event = {0};
    
    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_QUIT)
      {
        programRunning = false;
        break;
      }
      else if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
      {
        if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN)
        {
          emu.Reset();
          emu.LoadProgram(program.get(), fileSize);
        }
        
        if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
        {
          programRunning = false;
          break;
        }
        
        uint8_t key = KeyCode(event.key.keysym.scancode); // Changed to scancode
        
        if(key != 0xFF)
        {
          emu.SetKeyState(key, event.type == SDL_KEYDOWN);
        }
        
        if(event.type == SDL_KEYDOWN && emu.WaitingForKey() && key != 0xFF)
        {
          emu.SetWaitingForKey(false);
          emu.mRegV[emu.mKeyReg] = key;
        }
      }
    }
    
    for(int i = 0; i < 6; ++i)
    {
      //LogEmu(emu);
      if(!emu.Tick()) break;
    }
    
    screen.Draw(emu.VideoMem());
    
    Sleep(16);
  }
  
  SDL_CloseAudioDevice(audioDevice);
  SDL_Quit();
  std::puts("Done");
}
