//
//  Emulator.h
//  Chip-8 Emu
//
//  Created by Daniel Hauser on 14.12.14.
//  Copyright (c) 2014 Daniel Hauser. All rights reserved.
//

#pragma once

#include <memory>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <cstring>
#include <chrono>

namespace C8
{
  class Emulator
  {
  public:
    // Globals
    static constexpr unsigned MemorySize = 4096;
    static constexpr unsigned ProgramOffset = 512;
    static constexpr unsigned StackSize = 16;
    
    static constexpr unsigned VideoWidth = 64;
    static constexpr unsigned VideoHeight = 32;
    
    //static constexpr unsigned BuzzerDuration = 100;
    enum {BuzzerDuration = 100};
    
    // Typenames
    using TimePoint = std::chrono::system_clock::time_point;
    using Duration = std::chrono::system_clock::duration;
  private:
    // Members
    bool mRunning = false;
    bool mWaitingForKey = false;
    std::unordered_map<uint8_t, bool> mKeyStates =
    {
      {0x0, false}, // 0
      {0x1, false}, // 1
      {0x2, false}, // 2
      {0x3, false}, // 3
      {0x4, false}, // 4
      {0x5, false}, // 5
      {0x6, false}, // 6
      {0x7, false}, // 7
      {0x8, false}, // 8
      {0x9, false}, // 9
      {0xA, false}, // A
      {0xB, false}, // B
      {0xC, false}, // C
      {0xD, false}, // D
      {0xE, false}, // E
      {0xF, false}, // F
    };
    
    TimePoint mStartTime = TimeNow();
    TimePoint mLastSpecialUpdateTime = TimeNow();
    TimePoint mLastBuzzerTime = TimeNow();
    
    // Methods
    void CriticalError(const std::string &msg);
    TimePoint TimeNow() const { return std::chrono::system_clock::now(); }
    Duration TimeRunning() const { return TimeNow() - mStartTime; }
    Duration LastSpecialUpdateTime() const { return TimeNow() - mLastSpecialUpdateTime; }
    Duration LastBuzzerTime() const { return TimeNow() - mLastBuzzerTime; }
    long long DurationMillis(const Duration &dur) const { return std::chrono::duration_cast<std::chrono::milliseconds>(dur).count(); }
    
    void UpdateSpecials();
  public:
    // Members
    std::unique_ptr<uint8_t[]> mMemory;
    uint16_t mStack[StackSize] = {0};
    uint8_t mVideoMem[VideoWidth * VideoHeight] = {0};

    int mKeyReg = 0;
    
    union
    {
      uint8_t mRegV[16] = {0};
      struct
      {
        uint8_t mRegV0;
        uint8_t mRegV1;
        uint8_t mRegV2;
        uint8_t mRegV3;
        uint8_t mRegV4;
        uint8_t mRegV5;
        uint8_t mRegV6;
        uint8_t mRegV7;
        uint8_t mRegV8;
        uint8_t mRegV9;
        uint8_t mRegVA;
        uint8_t mRegVB;
        uint8_t mRegVC;
        uint8_t mRegVD;
        uint8_t mRegVE;
        uint8_t mRegVF;
      };
    };
    
    uint16_t mRegI = 0;
    uint16_t mRegPC = ProgramOffset;
    uint8_t  mRegSP = 0;
    
    uint8_t mRegDT = 0;
    uint8_t mRegST = 0;
  public:
    // Getters & setters
    bool Running() const { return mRunning; }
    bool WaitingForKey() const { return mWaitingForKey; }
    void SetWaitingForKey(bool waiting) { mWaitingForKey = waiting; }
    const uint8_t *VideoMem() const { return mVideoMem; }
    
    // Ctors & dtor
    Emulator();
    
    // Methods
    inline uint16_t StackTop() const { return mStack[mRegSP]; }
    
    bool RunOp(uint16_t op);
    bool Tick();
    
    bool SetPixel(int x, int y, bool value);
    bool GetPixel(int x, int y);
    
    inline void ClearScreen() { memset(mVideoMem, 0, VideoWidth * VideoHeight); }
    
    void LoadProgram(const uint8_t *prog, size_t progSize);
    inline void Start() { mRunning = true; }
    
    void SetKeyState(uint8_t key, bool state) { mKeyStates[key] = state; }
    bool GetKeyState(uint8_t key);
    
    void Reset();
    
    bool ShouldPlaySound() const;
  };
}