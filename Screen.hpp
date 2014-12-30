//
//  Screen.h
//  Chip-8 Emu
//
//  Created by Daniel Hauser on 19.12.14.
//  Copyright (c) 2014 Daniel Hauser. All rights reserved.
//

#pragma once

#include <string>
#include <SDL2/SDL.h>

#include "Preferences.hpp"

namespace UI
{
  class Screen final
  {
  private:
    SDL_Window *mWindow = nullptr;
    SDL_Renderer *mRenderer = nullptr;
    SDL_Texture *mTexture = nullptr;
    
    int mTexWidth, mTexHeight;
    
    bool mSuccess = false;
    
    unsigned int mDrawColor = Preferences::RenderDrawColor();
    unsigned int mClearColor = Preferences::RenderClearColor();
  public:
    // Getters & setters
    bool Success() const { return mSuccess; }
    
    // Ctors & dtor
    Screen(const std::string &title, int width, int height, int texWidth, int texHeight);
    ~Screen();
    
    // Methods
    void Draw(const uint8_t *vram);
  };
}