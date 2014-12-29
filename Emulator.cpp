//
//  Emulator.cpp
//  Chip-8 Emu
//
//  Created by Daniel Hauser on 14.12.14.
//  Copyright (c) 2014 Daniel Hauser. All rights reserved.
//

#include "Emulator.hpp"

#include <cstdlib>
#include <algorithm>

#include "Instr.hpp"
#include "InstrSet.hpp"

static uint8_t fontData[] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80, // F
};

namespace C8
{
  Emulator::Emulator()
  : mMemory(new uint8_t[MemorySize])
  {
    // Write font data
    memcpy(mMemory.get(), fontData, sizeof(fontData));
    
    // Init
    mStartTime = mLastSpecialUpdateTime = TimeNow();
  }
  
  void Emulator::UpdateSpecials()
  {
    auto time = DurationMillis(LastSpecialUpdateTime());
    uint16_t delta = time / 16;
    if(delta < 0) return;
    
    auto oldST = mRegST;
    
    mRegDT = std::max(mRegDT - delta, 0);
    mRegST = std::max(mRegST - delta, 0);
    
    if(mRegST != oldST)
      mLastBuzzerTime = TimeNow();
    
    mLastSpecialUpdateTime = TimeNow();
  }
  
  bool Emulator::Tick()
  {
    UpdateSpecials();
    
    if(mWaitingForKey) return true;
    
    uint16_t op = mMemory[mRegPC] << 8 | mMemory[mRegPC + 1];
    //std::printf("<Opcode %X>\n", op);
    if(!RunOp(op))
    {
      CriticalError("Couldn't execute opcode.");
      std::printf("Opcode: %X\n", op);
      return false;
    }
    
    mRegPC += 2;
    return true;
  }
  
  void Emulator::CriticalError(const std::string &msg)
  {
    std::printf("CRITICAL EMULATOR ERROR: %s\n", msg.c_str());
    mRunning = false;
  }
  
  bool Emulator::RunOp(uint16_t op)
  {
    Instr ins = OpInstrId(op);
    
    switch(ins)
    {
      case Instr::CLS: InstrSet::Cls(*this, op); break;
      case Instr::RET: InstrSet::Ret(*this, op); break;
      case Instr::JP_ADDR: InstrSet::Jp_Addr(*this, op); break;
      case Instr::CALL: InstrSet::Call(*this, op); break;
      case Instr::SE_VX_BYTE: InstrSet::Se_Vx_Byte(*this, op); break;
      case Instr::SNE_VX_BYTE: InstrSet::Sne_Vx_Byte(*this, op); break;
      case Instr::SE_VX_VY: InstrSet::Se_Vx_Vy(*this, op); break;
      case Instr::LD_VX_BYTE: InstrSet::Ld_Vx_Byte(*this, op); break;
      case Instr::ADD_VX_BYTE: InstrSet::Add_Vx_Byte(*this, op); break;
      case Instr::LD_VX_VY: InstrSet::Ld_Vx_Vy(*this, op); break;
      case Instr::OR_VX_VY: InstrSet::Or_Vx_Vy(*this, op); break;
      case Instr::AND_VX_VY: InstrSet::And_Vx_Vy(*this, op); break;
      case Instr::XOR_VX_VY: InstrSet::Xor_Vx_Vy(*this, op); break;
      case Instr::ADD_VX_VY: InstrSet::Add_Vx_Vy(*this, op); break;
      case Instr::SUB_VX_VY: InstrSet::Sub_Vx_Vy(*this, op); break;
      case Instr::SHR_VX: InstrSet::Shr_Vx(*this, op); break;
      case Instr::SUBN_VX_VY: InstrSet::Subn_Vx_Vy(*this, op); break;
      case Instr::SHL_VX: InstrSet::Shl_Vx(*this, op); break;
      case Instr::SNE_VX_VY: InstrSet::Sne_Vx_Vy(*this, op); break;
      case Instr::LD_I_ADDR: InstrSet::Ld_I_Addr(*this, op); break;
      case Instr::JP_V0_ADDR: InstrSet::Jp_V0_Addr(*this, op); break;
      case Instr::RND: InstrSet::Rnd(*this, op); break;
      case Instr::DRW: InstrSet::Drw(*this, op); break;
      case Instr::SKP: InstrSet::Skp(*this, op); break;
      case Instr::SKNP: InstrSet::Sknp(*this, op); break;
      case Instr::LD_VX_DT: InstrSet::Ld_Vx_Dt(*this, op); break;
      case Instr::LD_VX_K: InstrSet::Ld_Vx_K(*this, op); break;
      case Instr::LD_DT_VX: InstrSet::Ld_Dt_Vx(*this, op); break;
      case Instr::LD_ST_VX: InstrSet::Ld_St_Vx(*this, op); break;
      case Instr::ADD_I_VX: InstrSet::Add_I_Vx(*this, op); break;
      case Instr::LD_F_VX: InstrSet::Ld_F_Vx(*this, op); break;
      case Instr::LD_B_VX: InstrSet::Ld_B_Vx(*this, op); break;
      case Instr::LD_I_VX: InstrSet::Ld_I_Vx(*this, op); break;
      case Instr::LD_VX_I: InstrSet::Ld_Vx_I(*this, op); break;
      default: return false;
    }
    
    return true;
  }
  
  bool Emulator::SetPixel(int x, int y, bool value)
  {
    const int index = VideoWidth * y + x;
    bool ret = false;
    
    if(x >= 0 && x < VideoWidth && y >= 0 && y < VideoHeight)
    {
      const uint8_t old = mVideoMem[index];
      const uint8_t val = (uint8_t)value;
      mVideoMem[index] ^= val;
      if(old == 1 && val == 1) ret = true;
    }
    
    return ret;
  }
  
  bool Emulator::GetPixel(int x, int y)
  {
    if(x > 0 && x < VideoWidth && y > 0 && y < VideoHeight)
      return mVideoMem[VideoWidth * y + x];
    return false;
  }
  
  void Emulator::LoadProgram(const uint8_t *prog, size_t progSize)
  {
    memcpy(&mMemory[ProgramOffset], prog, progSize);
  }
  
  bool Emulator::GetKeyState(uint8_t key)
  {
    auto iter = mKeyStates.find(key);
    if(iter == mKeyStates.end()) return false;
    return iter->second;
  }
  
  void Emulator::Reset()
  {
    // Clear main memory
    memset(mMemory.get(), 0, MemorySize);
    
    // Write font data
    memcpy(mMemory.get(), fontData, sizeof(fontData));
    
    // Clear stack
    memset(mStack, 0, 2 * StackSize);
    
    // Clear video memory
    memset(mVideoMem, 0, VideoWidth * VideoHeight);
    
    // Registers
    memset(mRegV, 0, 16);
    mRegI = mRegSP = mRegDT = mRegST = 0;
    mRegPC = ProgramOffset;
    
    // Other
    mKeyReg = 0;
    mWaitingForKey = false;
    
    // Init
    mStartTime = mLastSpecialUpdateTime = mLastBuzzerTime = TimeNow();
  }
  
  bool Emulator::ShouldPlaySound() const
  {
    return TimeNow() <= mLastBuzzerTime + std::chrono::milliseconds(BuzzerDuration);
  }
}