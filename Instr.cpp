//
//  Instr.cpp
//  Chip-8 Emu
//
//  Created by Daniel Hauser on 14.12.14.
//  Copyright (c) 2014 Daniel Hauser. All rights reserved.
//

#include "Instr.hpp"

#include <cstdio>

namespace C8
{
  Instr OpInstrId(uint16_t op)
  {
    const unsigned h = op >> 12;
    const unsigned l = op & 0xF;
    
    // 0x0 family
    if(op == 0x00E0) return Instr::CLS;
    if(op == 0x00EE) return Instr::RET;
    
    // 0x1 - 0x7 families
    switch(h)
    {
      case 0x1: return Instr::JP_ADDR;
      case 0x2: return Instr::CALL;
      case 0x3: return Instr::SE_VX_BYTE;
      case 0x4: return Instr::SNE_VX_BYTE;
      case 0x5: return Instr::SE_VX_VY;
      case 0x6: return Instr::LD_VX_BYTE;
      case 0x7: return Instr::ADD_VX_BYTE;
    }
    
    // 0x8 family
    if(h == 0x8)
    {
      switch(l)
      {
        case 0x0: return Instr::LD_VX_VY;
        case 0x1: return Instr::OR_VX_VY;
        case 0x2: return Instr::AND_VX_VY;
        case 0x3: return Instr::XOR_VX_VY;
        case 0x4: return Instr::ADD_VX_VY;
        case 0x5: return Instr::SUB_VX_VY;
        case 0x6: return Instr::SHR_VX;
        case 0x7: return Instr::SUBN_VX_VY;
        case 0xE: return Instr::SHL_VX;
      }
    }
    
    // 0x9 - 0xD families
    switch(h)
    {
      case 0x9: return Instr::SNE_VX_VY;
      case 0xA: return Instr::LD_I_ADDR;
      case 0xB: return Instr::JP_V0_ADDR;
      case 0xC: return Instr::RND;
      case 0xD: return Instr::DRW;
    }
    
    // 0xE family
    if(h == 0xE)
    {
      switch(l)
      {
        case 0xE: return Instr::SKP;
        case 0x1: return Instr::SKNP;
      }
    }
    
    // 0xF family
    switch(l)
    {
      case 0x7: return Instr::LD_VX_DT;
      case 0xA: return Instr::LD_VX_K;
      case 0x8: return Instr::LD_ST_VX;
      case 0xE: return Instr::ADD_I_VX;
      case 0x9: return Instr::LD_F_VX;
      case 0x3: return Instr::LD_B_VX;
      case 0x5:
      {
        const unsigned lh = op >> 4 & 0xF;
        if(lh == 0x1) return Instr::LD_DT_VX;
        if(lh == 0x5) return Instr::LD_I_VX;
        if(lh == 0x6) return Instr::LD_VX_I;
      }
    }
    
    std::printf("Invalid opcode: %X\n", op);
    return Instr::NIL;
  }
}