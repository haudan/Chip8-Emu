//
//  Preferences.cpp
//  Chip-8 Emu
//
//  Created by Daniel Hauser on 30.12.14.
//  Copyright (c) 2014 Daniel Hauser. All rights reserved.
//

#include "Preferences.hpp"

#include <memory>
#include <cstdio>
#include <cstdint>

static uint8_t HexChToNibble(char hex)
{
  return (hex >= '0' && hex <= '9') ? hex - '0'
  : (hex >= 'a' && hex <= 'f') ? hex - 'a' + 10
  : (hex >= 'A' && hex <= 'F') ? hex - 'A' + 10
  : 0;
}

static uint8_t HexStrToByte(const std::string &str)
{
  return HexChToNibble(str[0]) << 4 | HexChToNibble(str[1]);
}

static unsigned int ParseColorStr(const std::string &str)
{
  unsigned int color = 0;
  
  for(int i = 0; i < str.length(); i += 2)
  {
    auto byte = HexStrToByte(str.substr(i, 2));
    color |= byte;
    if(i < str.length() - 2)
      color <<= 8;
  }
  
  return color;
}

namespace Preferences
{
  static rapidjson::Document doc;
  
  bool Load(const std::string &file)
  {
    FILE *fp = fopen(file.c_str(), "rb");
    if(!fp) return false;
    
    fseek(fp, 0, SEEK_END);
    auto fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    std::unique_ptr<char[]> fileContent(new char[fileSize]);
    fread(fileContent.get(), 1, fileSize, fp);
    fclose(fp);
    
    doc.Parse(fileContent.get());
    return doc.IsObject();
  }
  
  rapidjson::Value &Get(const std::string &name)
  {
    return doc[name.c_str()];
  }
  
  // Helpers
  
  unsigned int AsHex(const char *name, unsigned int defaultHex)
  {
    if(!doc.IsObject()) return defaultHex;
    auto iter = doc.FindMember(name);
    if(iter == doc.MemberEnd()) return defaultHex;
    return ParseColorStr(iter->value.GetString());
  }
  
  double AsNumber(const char *name, double defaultNumber)
  {
    if(!doc.IsObject()) return defaultNumber;
    auto iter = doc.FindMember(name);
    if(iter == doc.MemberEnd()) return defaultNumber;
    return iter->value.GetDouble();
  }
}