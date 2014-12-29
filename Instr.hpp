//
//  Instr.h
//  Chip-8 Emu
//
//  Created by Daniel Hauser on 14.12.14.
//  Copyright (c) 2014 Daniel Hauser. All rights reserved.
//

#pragma once

#include <cstdint>

namespace C8
{
  enum class Instr
  {
    CLS = 0,
    RET,
    JP_ADDR,
    CALL,
    SE_VX_BYTE,
    SNE_VX_BYTE,
    SE_VX_VY,
    LD_VX_BYTE,
    ADD_VX_BYTE,
    LD_VX_VY,
    OR_VX_VY, // 10
    AND_VX_VY,
    XOR_VX_VY,
    ADD_VX_VY,
    SUB_VX_VY,
    SHR_VX,
    SUBN_VX_VY,
    SHL_VX,
    SNE_VX_VY,
    LD_I_ADDR,
    JP_V0_ADDR, // 20
    RND,
    DRW,
    SKP,
    SKNP,
    LD_VX_DT,
    LD_VX_K,
    LD_DT_VX,
    LD_ST_VX,
    ADD_I_VX,
    LD_F_VX, // 30
    LD_B_VX,
    LD_I_VX,
    LD_VX_I,
    NIL
  };
  
  Instr OpInstrId(uint16_t op);
}