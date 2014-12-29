//
//  Screen.cpp
//  Chip-8 Emu
//
//  Created by Daniel Hauser on 19.12.14.
//  Copyright (c) 2014 Daniel Hauser. All rights reserved.
//

#include "Screen.hpp"

namespace UI
{
  Screen::Screen(const std::string &title, int width, int height,
                 int texWidth, int texHeight)
  : mTexWidth(texWidth), mTexHeight(texHeight)
  {
    mSuccess = false;
    
    mWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               width, height, 0);
    if(!mWindow) return;
    
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    if(!mRenderer) return;
    
    mTexture = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                 texWidth, texHeight);
    if(!mTexture) return;
    
    mSuccess = true;
  }
  
  Screen::~Screen()
  {
    if(mTexture) SDL_DestroyTexture(mTexture);
    if(mRenderer) SDL_DestroyRenderer(mRenderer);
    if(mWindow) SDL_DestroyWindow(mWindow);
  }
  
  void Screen::Draw(const uint8_t *vram)
  {
    uint32_t pixels[mTexWidth * mTexHeight];
    
    for(int i = 0; i < mTexWidth * mTexHeight; ++i)
    {
      pixels[i] = vram[i] ? 0xFFFFFFFF : 0xFF000000;
    }
    
    SDL_UpdateTexture(mTexture, nullptr, pixels, 4 * mTexWidth);
    SDL_RenderCopy(mRenderer, mTexture, nullptr, nullptr);
    SDL_RenderPresent(mRenderer);
  }
  
  
}