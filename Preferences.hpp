//
//  Preferences.h
//  Chip-8 Emu
//
//  Created by Daniel Hauser on 30.12.14.
//  Copyright (c) 2014 Daniel Hauser. All rights reserved.
//

#pragma once

#include <rapidjson/document.h>
#include <string>

namespace Preferences
{
  bool Load(const std::string &file);
  rapidjson::Value &Get(const std::string &name);
  
  // Helpers
  
  double AsNumber(const char *name, double defaultNumber);
  unsigned int AsHex(const char *name, unsigned int defaultHex);
  bool AsBool(const char *name, bool defaultBool);
  
  // Settings
  
  inline unsigned int RenderDrawColor() { return AsHex("RenderDrawColor", 0xFFFFFFFF); }
  inline unsigned int RenderClearColor() { return AsHex("RenderClearColor", 0xFF000000); }
  inline float Volume() { return AsNumber("Volume", 0.5); }
  inline int Ticks() { return AsNumber("Ticks", 8); }
  inline bool Resizable() { return AsBool("Resizable", true); }
  inline int Width() { return AsNumber("Width", 960); }
  inline int Height() { return AsNumber("Height", 480); }
}