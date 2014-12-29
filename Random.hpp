//
//  Random.h
//  Chip-8 Emu
//
//  Created by Daniel Hauser on 29.12.14.
//  Copyright (c) 2014 Daniel Hauser. All rights reserved.
//

#pragma once

#include <random>
#include <chrono>

unsigned int TimeSinceEpoch();

extern std::mt19937 _randEngine;

template <typename T>
inline T RandReal(T min, T max)
{
  return std::uniform_real_distribution<T>(min, max)(_randEngine);
}

template <typename T>
inline T RandInt(T min, T max)
{
  return std::uniform_int_distribution<T>(min, max)(_randEngine);
}