//
//  InstrSet.hpp
//  Chip-8 Emu
//
//  Created by Daniel Hauser on 19.12.14.
//  Copyright (c) 2014 Daniel Hauser. All rights reserved.
//

#pragma once

#include "Emulator.hpp"

#include <random>
#include <chrono>

#include "Random.hpp"

namespace C8
{
  inline uint8_t RegX(uint16_t op) { return (op >> 8) & 0xF; }
  inline uint8_t RegY(uint16_t op) { return (op >> 4) & 0xF; }
  
  constexpr unsigned SkipPCDelta = 2;
  
  namespace InstrSet
  {
    inline void Cls(Emulator &emu, uint16_t op) { emu.ClearScreen(); }
    
    inline void Ret(Emulator &emu, uint16_t op)
    {
      emu.mRegPC = emu.StackTop();
      --emu.mRegSP;
    }
    
    inline void Jp_Addr(Emulator &emu, uint16_t op)
    {
      emu.mRegPC = (op & 0x0FFF) - SkipPCDelta; // Changed
    }
    
    inline void Call(Emulator &emu, uint16_t op)
    {
      emu.mStack[++emu.mRegSP] = emu.mRegPC;
      emu.mRegPC = (op & 0x0FFF) - SkipPCDelta; // Changed
    }
    
    inline void Se_Vx_Byte(Emulator &emu, uint16_t op)
    {
      if(emu.mRegV[RegX(op)] == (op & 0xFF)) emu.mRegPC += SkipPCDelta;
    }
    
    inline void Sne_Vx_Byte(Emulator &emu, uint16_t op)
    {
      if(emu.mRegV[RegX(op)] != (op & 0xFF)) emu.mRegPC += SkipPCDelta;
    }
    
    inline void Se_Vx_Vy(Emulator &emu, uint16_t op)
    {
      if(emu.mRegV[RegX(op)] == emu.mRegV[RegY(op)]) emu.mRegPC += SkipPCDelta;
    }
    
    inline void Ld_Vx_Byte(Emulator &emu, uint16_t op)
    {
      emu.mRegV[RegX(op)] = op & 0xFF;
    }
    
    inline void Add_Vx_Byte(Emulator &emu, uint16_t op)
    {
      emu.mRegV[RegX(op)] += op & 0xFF;
    }
    
    inline void Ld_Vx_Vy(Emulator &emu, uint16_t op)
    {
      emu.mRegV[RegX(op)] = emu.mRegV[RegY(op)];
    }
    
    inline void Or_Vx_Vy(Emulator &emu, uint16_t op)
    {
      emu.mRegV[RegX(op)] |= emu.mRegV[RegY(op)];
    }
    
    inline void And_Vx_Vy(Emulator &emu, uint16_t op)
    {
      emu.mRegV[RegX(op)] &= emu.mRegV[RegY(op)];
    }
    
    inline void Xor_Vx_Vy(Emulator &emu, uint16_t op)
    {
      emu.mRegV[RegX(op)] ^= emu.mRegV[RegY(op)];
    }
    
    inline void Add_Vx_Vy(Emulator &emu, uint16_t op)
    {
      unsigned res = emu.mRegV[RegX(op)] += emu.mRegV[RegY(op)];
      if(res > 255) emu.mRegVF = 1;
      else emu.mRegVF = 0;
      emu.mRegV[RegX(op)] = res & 0xFF;
    }
    
    inline void Sub_Vx_Vy(Emulator &emu, uint16_t op)
    {
      if(emu.mRegV[RegX(op)] > emu.mRegV[RegY(op)]) emu.mRegVF = 1;
      else emu.mRegVF = 0;
      emu.mRegV[RegX(op)] -= emu.mRegV[RegY(op)];
    }
    
    inline void Shr_Vx(Emulator &emu, uint16_t op)
    {
      if(emu.mRegV[RegX(op)] & 1) emu.mRegVF = 1;
      else emu.mRegVF = 0;
      emu.mRegV[RegX(op)] >>= 1;
    }
    
    inline void Subn_Vx_Vy(Emulator &emu, uint16_t op)
    {
      if(emu.mRegV[RegY(op)] > emu.mRegV[RegX(op)]) emu.mRegVF = 1;
      else emu.mRegVF = 0;
      emu.mRegV[RegX(op)] = emu.mRegV[RegY(op)] - emu.mRegV[RegX(op)];
    }
    
    inline void Shl_Vx(Emulator &emu, uint16_t op)
    {
      if(emu.mRegV[RegX(op)] & 0x80) emu.mRegVF = 1;
      else emu.mRegVF = 0;
      emu.mRegV[RegX(op)] <<= 1;
    }
    
    inline void Sne_Vx_Vy(Emulator &emu, uint16_t op)
    {
      if(emu.mRegV[RegX(op)] != emu.mRegV[RegY(op)]) emu.mRegPC += SkipPCDelta;
    }
    
    inline void Ld_I_Addr(Emulator &emu, uint16_t op)
    {
      emu.mRegI = op & 0x0FFF;
    }
    
    inline void Jp_V0_Addr(Emulator &emu, uint16_t op)
    {
      emu.mRegPC = emu.mRegV0 + (op & 0x0FFF);
    }
    
    inline void Rnd(Emulator &emu, uint16_t op)
    {
      emu.mRegV[RegX(op)] = RandInt<uint8_t>(0, 255) & (op & 0xFF);
    }
    
    inline void Drw(Emulator &emu, uint16_t op)
    {
      bool coll = false;
      
      const int num = op & 0xF;
      for(int i = 0; i < num; ++i)
      {
        const uint8_t row = emu.mMemory[emu.mRegI + i];
        
        for(int j = 0; j < 8; ++j)
        {
          if(emu.SetPixel(emu.mRegV[RegX(op)] + j, emu.mRegV[RegY(op)] + i, row & (0x80 >> j)))
            coll = true;
        }
      }
      
      if(coll) emu.mRegVF = 1;
      else emu.mRegVF = 0;
    }
    
    inline void Skp(Emulator &emu, uint16_t op)
    {
      if(emu.GetKeyState(emu.mRegV[RegX(op)])) emu.mRegPC += SkipPCDelta;
    }
    
    inline void Sknp(Emulator &emu, uint16_t op)
    {
      if(!emu.GetKeyState(emu.mRegV[RegX(op)])) emu.mRegPC += SkipPCDelta;
    }
    
    inline void Ld_Vx_Dt(Emulator &emu, uint16_t op)
    {
      int reg = RegX(op);
      emu.mRegV[reg] = emu.mRegDT;
    }
    
    inline void Ld_Vx_K(Emulator &emu, uint16_t op)
    {
      emu.SetWaitingForKey(true);
      emu.mKeyReg = RegX(op);
    }
    
    inline void Ld_Dt_Vx(Emulator &emu, uint16_t op)
    {
      emu.mRegDT = emu.mRegV[RegX(op)];
    }
    
    inline void Ld_St_Vx(Emulator &emu, uint16_t op)
    {
      emu.mRegST = emu.mRegV[RegX(op)];
    }
    
    inline void Add_I_Vx(Emulator &emu, uint16_t op)
    {
      emu.mRegI += emu.mRegV[RegX(op)];
    }
    
    inline void Ld_F_Vx(Emulator &emu, uint16_t op)
    {
      emu.mRegI = emu.mRegV[RegX(op)] * 5;
    }
    
    inline void Ld_B_Vx(Emulator &emu, uint16_t op)
    {
      const int num = emu.mRegV[RegX(op)];
      emu.mMemory[emu.mRegI + 0] = num / 100;
      emu.mMemory[emu.mRegI + 1] = num % 100 / 10;
      emu.mMemory[emu.mRegI + 2] = num % 100 % 10;
    }
    
    inline void Ld_I_Vx(Emulator &emu, uint16_t op)
    {
      memcpy(&emu.mMemory[emu.mRegI], emu.mRegV, RegX(op) + 1);
    }
    
    inline void Ld_Vx_I(Emulator &emu, uint16_t op)
    {
      memcpy(emu.mRegV, &emu.mMemory[emu.mRegI], RegX(op) + 1);
    }
  }
}