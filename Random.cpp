//
//  Random.cpp
//  Chip-8 Emu
//
//  Created by Daniel Hauser on 29.12.14.
//  Copyright (c) 2014 Daniel Hauser. All rights reserved.
//

#include "Random.hpp"

unsigned int TimeSinceEpoch()
{
  return (unsigned int)std::chrono::system_clock::now()
  .time_since_epoch().count();
}

std::mt19937 _randEngine{TimeSinceEpoch()};