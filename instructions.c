
#include <stdint.h>

enum InstructionType {
  NOP,
  LXI,
  STAX,
  INX,
  INR,
  DCR,
  MVI,
  RLC,
  DAD,
  LDAX,
  DCX,
  RRC,
  RAL,
  DAA,
  RAR,
  SHLD,
  LHLD,
  CMA,
  STA,
  STC,
  LDA,
  CMC,
  MOV,
  HLT,
  ADD,
  ADC,
  SUB,
  SBB,
  ANA,
  XRA,
  ORA,
  CMP,
  RNZ,
  POP,
  JNZ,
  CNZ,
  PUSH,
  ADI,
  RST,
  RZ,
  RET,
  JZ,
  JMP,
  CZ,
  CALL,
  ACI,
  RNC,
  JNC,
  OUT,
  CNC,
  SUI,
  RC,
  JC,
  IN,
  CC,
  SBI,
  RPO,
  JPO,
  XTHL,
  CPO,
  ANI,
  RPE,
  PCHL,
  JPE,
  XCHG,
  CPE,
  XRI,
  RP,
  JP,
  DI,
  CP,
  ORI,
  RM,
  SPHL,
  JM,
  EI,
  CM,
  CPI,
};

enum InstructionArgument {
  REG_A,
  REG_B,
  REG_C,
  REG_D,
  REG_E,
  REG_F,
  REG_H,
  REG_L,
  REG_M,
  D8,
  D16,
  A16,
  SP,
  PSW
};

struct Instruction {
  char *name;
  enum InstructionType type;
  enum InstructionArgument arg_a;
  enum InstructionArgument arg_b;
  uint8_t code;
  uint8_t bytes;
  uint8_t clock_cycles;
};

struct Instruction instruction_table[] = {
    {.name = "NOP", .code = 0x0, .bytes = 1, .clock_cycles = 4, .type = NOP},
    {.name = "LXI B,d16",
     .code = 0x1,
     .bytes = 3,
     .clock_cycles = 1,
     .type = LXI,
     .arg_a = REG_B,
     .arg_b = D16},
    {.name = "STAX B",
     .code = 0x2,
     .bytes = 1,
     .clock_cycles = 7,
     .type = STAX,
     .arg_a = REG_B},
    {.name = "INX B",
     .code = 0x3,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INX,
     .arg_a = REG_B},
    {.name = "INR B",
     .code = 0x4,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INR,
     .arg_a = REG_B},
    {.name = "DCR B",
     .code = 0x5,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCR,
     .arg_a = REG_B},
    {.name = "MVI B,d8",
     .code = 0x6,
     .bytes = 2,
     .clock_cycles = 7,
     .type = MVI,
     .arg_a = REG_B,
     .arg_b = D8},
    {.name = "RLC", .code = 0x7, .bytes = 1, .clock_cycles = 4, .type = RLC},
    {.name = "*NOP", .code = 0x8, .bytes = 1, .clock_cycles = 4, .type = NOP},
    {.name = "DAD B",
     .code = 0x9,
     .bytes = 1,
     .clock_cycles = 1,
     .type = DAD,
     .arg_a = REG_B},
    {.name = "LDAX B",
     .code = 0xA,
     .bytes = 1,
     .clock_cycles = 7,
     .type = LDAX,
     .arg_a = REG_B},
    {.name = "DCX B",
     .code = 0xB,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCX,
     .arg_a = REG_B},
    {.name = "INR C",
     .code = 0xC,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INR,
     .arg_a = REG_C},
    {.name = "DCR C",
     .code = 0xD,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCR,
     .arg_a = REG_C},
    {.name = "MVI C,d8",
     .code = 0xE,
     .bytes = 2,
     .clock_cycles = 7,
     .type = MVI,
     .arg_a = REG_C,
     .arg_b = D8},
    {.name = "RRC", .code = 0xF, .bytes = 1, .clock_cycles = 4, .type = RRC},
    {.name = "*NOP", .code = 0x10, .bytes = 1, .clock_cycles = 4, .type = NOP},
    {.name = "LXI D,d16",
     .code = 0x11,
     .bytes = 3,
     .clock_cycles = 1,
     .type = LXI,
     .arg_a = REG_D,
     .arg_b = D16},
    {.name = "STAX D",
     .code = 0x12,
     .bytes = 1,
     .clock_cycles = 7,
     .type = STAX,
     .arg_a = REG_D},
    {.name = "INX D",
     .code = 0x13,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INX,
     .arg_a = REG_D},
    {.name = "INR D",
     .code = 0x14,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INR,
     .arg_a = REG_D},
    {.name = "DCR D",
     .code = 0x15,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCR,
     .arg_a = REG_D},
    {.name = "MVI D,d8",
     .code = 0x16,
     .bytes = 2,
     .clock_cycles = 7,
     .type = MVI,
     .arg_a = REG_D,
     .arg_b = D8},
    {.name = "RAL", .code = 0x17, .bytes = 1, .clock_cycles = 4, .type = RAL},
    {.name = "*NOP", .code = 0x18, .bytes = 1, .clock_cycles = 4, .type = NOP},
    {.name = "DAD D",
     .code = 0x19,
     .bytes = 1,
     .clock_cycles = 1,
     .type = DAD,
     .arg_a = REG_D},
    {.name = "LDAX D",
     .code = 0x1A,
     .bytes = 1,
     .clock_cycles = 7,
     .type = LDAX,
     .arg_a = REG_D},
    {.name = "DCX D",
     .code = 0x1B,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCX,
     .arg_a = REG_D},
    {.name = "INR E",
     .code = 0x1C,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INR,
     .arg_a = REG_E},
    {.name = "DCR E",
     .code = 0x1D,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCR,
     .arg_a = REG_E},
    {.name = "MVI E,d8",
     .code = 0x1E,
     .bytes = 2,
     .clock_cycles = 7,
     .type = MVI,
     .arg_a = REG_E,
     .arg_b = D8},
    {.name = "RAR", .code = 0x1F, .bytes = 1, .clock_cycles = 4, .type = RAR},
    {.name = "*NOP", .code = 0x20, .bytes = 1, .clock_cycles = 4, .type = NOP},
    {.name = "LXI H,d16",
     .code = 0x21,
     .bytes = 3,
     .clock_cycles = 1,
     .type = LXI,
     .arg_a = REG_H,
     .arg_b = D16},
    {.name = "SHLD a16",
     .code = 0x22,
     .bytes = 3,
     .clock_cycles = 1,
     .type = SHLD,
     .arg_a = A16},
    {.name = "INX H",
     .code = 0x23,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INX,
     .arg_a = REG_H},
    {.name = "INR H",
     .code = 0x24,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INR,
     .arg_a = REG_H},
    {.name = "DCR H",
     .code = 0x25,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCR,
     .arg_a = REG_H},
    {.name = "MVI H,d8",
     .code = 0x26,
     .bytes = 2,
     .clock_cycles = 7,
     .type = MVI,
     .arg_a = REG_H,
     .arg_b = D8},
    {.name = "DAA", .code = 0x27, .bytes = 1, .clock_cycles = 4, .type = DAA},
    {.name = "*NOP", .code = 0x28, .bytes = 1, .clock_cycles = 4, .type = NOP},
    {.name = "DAD H",
     .code = 0x29,
     .bytes = 1,
     .clock_cycles = 1,
     .type = DAD,
     .arg_a = REG_H},
    {.name = "LHLD a16",
     .code = 0x2A,
     .bytes = 3,
     .clock_cycles = 1,
     .type = LHLD,
     .arg_a = A16},
    {.name = "DCX H",
     .code = 0x2B,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCX,
     .arg_a = REG_H},
    {.name = "INR L",
     .code = 0x2C,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INR,
     .arg_a = REG_L},
    {.name = "DCR L",
     .code = 0x2D,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCR,
     .arg_a = REG_L},
    {.name = "MVI L,d8",
     .code = 0x2E,
     .bytes = 2,
     .clock_cycles = 7,
     .type = MVI,
     .arg_a = REG_L,
     .arg_b = D8},
    {.name = "CMA", .code = 0x2F, .bytes = 1, .clock_cycles = 4, .type = CMA},
    {.name = "*NOP", .code = 0x30, .bytes = 1, .clock_cycles = 4, .type = NOP},
    {.name = "LXI SP,d16",
     .code = 0x31,
     .bytes = 3,
     .clock_cycles = 1,
     .type = LXI,
     .arg_a = SP,
     .arg_b = D16},
    {.name = "STA a16",
     .code = 0x32,
     .bytes = 3,
     .clock_cycles = 1,
     .type = STA,
     .arg_a = A16},
    {.name = "INX SP",
     .code = 0x33,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INX,
     .arg_a = SP},
    {.name = "INR M",
     .code = 0x34,
     .bytes = 1,
     .clock_cycles = 1,
     .type = INR,
     .arg_a = REG_M},
    {.name = "DCR M",
     .code = 0x35,
     .bytes = 1,
     .clock_cycles = 1,
     .type = DCR,
     .arg_a = REG_M},
    {.name = "MVI M,d8",
     .code = 0x36,
     .bytes = 2,
     .clock_cycles = 1,
     .type = MVI,
     .arg_a = REG_M,
     .arg_b = D8},
    {.name = "STC", .code = 0x37, .bytes = 1, .clock_cycles = 4, .type = STC},
    {.name = "*NOP", .code = 0x38, .bytes = 1, .clock_cycles = 4, .type = NOP},
    {.name = "DAD SP",
     .code = 0x39,
     .bytes = 1,
     .clock_cycles = 1,
     .type = DAD,
     .arg_a = SP},
    {.name = "LDA a16",
     .code = 0x3A,
     .bytes = 3,
     .clock_cycles = 1,
     .type = LDA,
     .arg_a = A16},
    {.name = "DCX SP",
     .code = 0x3B,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCX,
     .arg_a = SP},
    {.name = "INR A",
     .code = 0x3C,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INR,
     .arg_a = REG_A},
    {.name = "DCR A",
     .code = 0x3D,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCR,
     .arg_a = REG_A},
    {.name = "MVI A,d8",
     .code = 0x3E,
     .bytes = 2,
     .clock_cycles = 7,
     .type = MVI,
     .arg_a = REG_A,
     .arg_b = D8},
    {.name = "CMC", .code = 0x3F, .bytes = 1, .clock_cycles = 4, .type = CMC},
    {.name = "MOV B,B",
     .code = 0x40,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_B,
     .arg_b = REG_B},
    {.name = "MOV B,C",
     .code = 0x41,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_B,
     .arg_b = REG_C},
    {.name = "MOV B,D",
     .code = 0x42,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_B,
     .arg_b = REG_D},
    {.name = "MOV B,E",
     .code = 0x43,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_B,
     .arg_b = REG_E},
    {.name = "MOV B,H",
     .code = 0x44,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_B,
     .arg_b = REG_H},
    {.name = "MOV B,L",
     .code = 0x45,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_B,
     .arg_b = REG_L},
    {.name = "MOV B,M",
     .code = 0x46,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_B,
     .arg_b = REG_M},
    {.name = "MOV B,A",
     .code = 0x47,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_B,
     .arg_b = REG_A},
    {.name = "MOV C,B",
     .code = 0x48,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_C,
     .arg_b = REG_B},
    {.name = "MOV C,C",
     .code = 0x49,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_C,
     .arg_b = REG_C},
    {.name = "MOV C,D",
     .code = 0x4A,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_C,
     .arg_b = REG_D},
    {.name = "MOV C,E",
     .code = 0x4B,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_C,
     .arg_b = REG_E},
    {.name = "MOV C,H",
     .code = 0x4C,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_C,
     .arg_b = REG_H},
    {.name = "MOV C,L",
     .code = 0x4D,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_C,
     .arg_b = REG_L},
    {.name = "MOV C,M",
     .code = 0x4E,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_C,
     .arg_b = REG_M},
    {.name = "MOV C,A",
     .code = 0x4F,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_C,
     .arg_b = REG_A},
    {.name = "MOV D,B",
     .code = 0x50,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_D,
     .arg_b = REG_B},
    {.name = "MOV D,C",
     .code = 0x51,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_D,
     .arg_b = REG_C},
    {.name = "MOV D,D",
     .code = 0x52,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_D,
     .arg_b = REG_D},
    {.name = "MOV D,E",
     .code = 0x53,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_D,
     .arg_b = REG_E},
    {.name = "MOV D,H",
     .code = 0x54,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_D,
     .arg_b = REG_H},
    {.name = "MOV D,L",
     .code = 0x55,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_D,
     .arg_b = REG_L},
    {.name = "MOV D,M",
     .code = 0x56,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_D,
     .arg_b = REG_M},
    {.name = "MOV D,A",
     .code = 0x57,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_D,
     .arg_b = REG_A},
    {.name = "MOV E,B",
     .code = 0x58,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_E,
     .arg_b = REG_B},
    {.name = "MOV E,C",
     .code = 0x59,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_E,
     .arg_b = REG_C},
    {.name = "MOV E,D",
     .code = 0x5A,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_E,
     .arg_b = REG_D},
    {.name = "MOV E,E",
     .code = 0x5B,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_E,
     .arg_b = REG_E},
    {.name = "MOV E,H",
     .code = 0x5C,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_E,
     .arg_b = REG_H},
    {.name = "MOV E,L",
     .code = 0x5D,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_E,
     .arg_b = REG_L},
    {.name = "MOV E,M",
     .code = 0x5E,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_E,
     .arg_b = REG_M},
    {.name = "MOV E,A",
     .code = 0x5F,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_E,
     .arg_b = REG_A},
    {.name = "MOV H,B",
     .code = 0x60,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_H,
     .arg_b = REG_B},
    {.name = "MOV H,C",
     .code = 0x61,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_H,
     .arg_b = REG_C},
    {.name = "MOV H,D",
     .code = 0x62,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_H,
     .arg_b = REG_D},
    {.name = "MOV H,E",
     .code = 0x63,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_H,
     .arg_b = REG_E},
    {.name = "MOV H,H",
     .code = 0x64,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_H,
     .arg_b = REG_H},
    {.name = "MOV H,L",
     .code = 0x65,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_H,
     .arg_b = REG_L},
    {.name = "MOV H,M",
     .code = 0x66,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_H,
     .arg_b = REG_M},
    {.name = "MOV H,A",
     .code = 0x67,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_H,
     .arg_b = REG_A},
    {.name = "MOV L,B",
     .code = 0x68,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_L,
     .arg_b = REG_B},
    {.name = "MOV L,C",
     .code = 0x69,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_L,
     .arg_b = REG_C},
    {.name = "MOV L,D",
     .code = 0x6A,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_L,
     .arg_b = REG_D},
    {.name = "MOV L,E",
     .code = 0x6B,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_L,
     .arg_b = REG_E},
    {.name = "MOV L,H",
     .code = 0x6C,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_L,
     .arg_b = REG_H},
    {.name = "MOV L,L",
     .code = 0x6D,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_L,
     .arg_b = REG_L},
    {.name = "MOV L,M",
     .code = 0x6E,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_L,
     .arg_b = REG_M},
    {.name = "MOV L,A",
     .code = 0x6F,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_L,
     .arg_b = REG_A},
    {.name = "MOV M,B",
     .code = 0x70,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_M,
     .arg_b = REG_B},
    {.name = "MOV M,C",
     .code = 0x71,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_M,
     .arg_b = REG_C},
    {.name = "MOV M,D",
     .code = 0x72,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_M,
     .arg_b = REG_D},
    {.name = "MOV M,E",
     .code = 0x73,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_M,
     .arg_b = REG_E},
    {.name = "MOV M,H",
     .code = 0x74,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_M,
     .arg_b = REG_H},
    {.name = "MOV M,L",
     .code = 0x75,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_M,
     .arg_b = REG_L},
    {.name = "HLT", .code = 0x76, .bytes = 1, .clock_cycles = 7, .type = HLT},
    {.name = "MOV M,A",
     .code = 0x77,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_M,
     .arg_b = REG_A},
    {.name = "MOV A,B",
     .code = 0x78,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_A,
     .arg_b = REG_B},
    {.name = "MOV A,C",
     .code = 0x79,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_A,
     .arg_b = REG_C},
    {.name = "MOV A,D",
     .code = 0x7A,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_A,
     .arg_b = REG_D},
    {.name = "MOV A,E",
     .code = 0x7B,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_A,
     .arg_b = REG_E},
    {.name = "MOV A,H",
     .code = 0x7C,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_A,
     .arg_b = REG_H},
    {.name = "MOV A,L",
     .code = 0x7D,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_A,
     .arg_b = REG_L},
    {.name = "MOV A,M",
     .code = 0x7E,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_A,
     .arg_b = REG_M},
    {.name = "MOV A,A",
     .code = 0x7F,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_A,
     .arg_b = REG_A},
    {.name = "ADD B",
     .code = 0x80,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADD,
     .arg_a = REG_B},
    {.name = "ADD C",
     .code = 0x81,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADD,
     .arg_a = REG_C},
    {.name = "ADD D",
     .code = 0x82,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADD,
     .arg_a = REG_D},
    {.name = "ADD E",
     .code = 0x83,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADD,
     .arg_a = REG_E},
    {.name = "ADD H",
     .code = 0x84,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADD,
     .arg_a = REG_H},
    {.name = "ADD L",
     .code = 0x85,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADD,
     .arg_a = REG_L},
    {.name = "ADD M",
     .code = 0x86,
     .bytes = 1,
     .clock_cycles = 7,
     .type = ADD,
     .arg_a = REG_M},
    {.name = "ADD A",
     .code = 0x87,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADD,
     .arg_a = REG_A},
    {.name = "ADC B",
     .code = 0x88,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADC,
     .arg_a = REG_B},
    {.name = "ADC C",
     .code = 0x89,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADC,
     .arg_a = REG_C},
    {.name = "ADC D",
     .code = 0x8A,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADC,
     .arg_a = REG_D},
    {.name = "ADC E",
     .code = 0x8B,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADC,
     .arg_a = REG_E},
    {.name = "ADC H",
     .code = 0x8C,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADC,
     .arg_a = REG_H},
    {.name = "ADC L",
     .code = 0x8D,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADC,
     .arg_a = REG_L},
    {.name = "ADC M",
     .code = 0x8E,
     .bytes = 1,
     .clock_cycles = 7,
     .type = ADC,
     .arg_a = REG_M},
    {.name = "ADC A",
     .code = 0x8F,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADC,
     .arg_a = REG_A},
    {.name = "SUB B",
     .code = 0x90,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SUB,
     .arg_a = REG_B},
    {.name = "SUB C",
     .code = 0x91,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SUB,
     .arg_a = REG_C},
    {.name = "SUB D",
     .code = 0x92,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SUB,
     .arg_a = REG_D},
    {.name = "SUB E",
     .code = 0x93,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SUB,
     .arg_a = REG_E},
    {.name = "SUB H",
     .code = 0x94,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SUB,
     .arg_a = REG_H},
    {.name = "SUB L",
     .code = 0x95,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SUB,
     .arg_a = REG_L},
    {.name = "SUB M",
     .code = 0x96,
     .bytes = 1,
     .clock_cycles = 7,
     .type = SUB,
     .arg_a = REG_M},
    {.name = "SUB A",
     .code = 0x97,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SUB,
     .arg_a = REG_A},
    {.name = "SBB B",
     .code = 0x98,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SBB,
     .arg_a = REG_B},
    {.name = "SBB C",
     .code = 0x99,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SBB,
     .arg_a = REG_C},
    {.name = "SBB D",
     .code = 0x9A,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SBB,
     .arg_a = REG_D},
    {.name = "SBB E",
     .code = 0x9B,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SBB,
     .arg_a = REG_E},
    {.name = "SBB H",
     .code = 0x9C,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SBB,
     .arg_a = REG_H},
    {.name = "SBB L",
     .code = 0x9D,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SBB,
     .arg_a = REG_L},
    {.name = "SBB M",
     .code = 0x9E,
     .bytes = 1,
     .clock_cycles = 7,
     .type = SBB,
     .arg_a = REG_M},
    {.name = "SBB A",
     .code = 0x9F,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SBB,
     .arg_a = REG_A},
    {.name = "ANA B",
     .code = 0xA0,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ANA,
     .arg_a = REG_B},
    {.name = "ANA C",
     .code = 0xA1,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ANA,
     .arg_a = REG_C},
    {.name = "ANA D",
     .code = 0xA2,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ANA,
     .arg_a = REG_D},
    {.name = "ANA E",
     .code = 0xA3,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ANA,
     .arg_a = REG_E},
    {.name = "ANA H",
     .code = 0xA4,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ANA,
     .arg_a = REG_H},
    {.name = "ANA L",
     .code = 0xA5,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ANA,
     .arg_a = REG_L},
    {.name = "ANA M",
     .code = 0xA6,
     .bytes = 1,
     .clock_cycles = 7,
     .type = ANA,
     .arg_a = REG_M},
    {.name = "ANA A",
     .code = 0xA7,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ANA,
     .arg_a = REG_A},
    {.name = "XRA B",
     .code = 0xA8,
     .bytes = 1,
     .clock_cycles = 4,
     .type = XRA,
     .arg_a = REG_B},
    {.name = "XRA C",
     .code = 0xA9,
     .bytes = 1,
     .clock_cycles = 4,
     .type = XRA,
     .arg_a = REG_C},
    {.name = "XRA D",
     .code = 0xAA,
     .bytes = 1,
     .clock_cycles = 4,
     .type = XRA,
     .arg_a = REG_D},
    {.name = "XRA E",
     .code = 0xAB,
     .bytes = 1,
     .clock_cycles = 4,
     .type = XRA,
     .arg_a = REG_E},
    {.name = "XRA H",
     .code = 0xAC,
     .bytes = 1,
     .clock_cycles = 4,
     .type = XRA,
     .arg_a = REG_H},
    {.name = "XRA L",
     .code = 0xAD,
     .bytes = 1,
     .clock_cycles = 4,
     .type = XRA,
     .arg_a = REG_L},
    {.name = "XRA M",
     .code = 0xAE,
     .bytes = 1,
     .clock_cycles = 7,
     .type = XRA,
     .arg_a = REG_M},
    {.name = "XRA A",
     .code = 0xAF,
     .bytes = 1,
     .clock_cycles = 4,
     .type = XRA,
     .arg_a = REG_A},
    {.name = "ORA B",
     .code = 0xB0,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ORA,
     .arg_a = REG_B},
    {.name = "ORA C",
     .code = 0xB1,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ORA,
     .arg_a = REG_C},
    {.name = "ORA D",
     .code = 0xB2,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ORA,
     .arg_a = REG_D},
    {.name = "ORA E",
     .code = 0xB3,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ORA,
     .arg_a = REG_E},
    {.name = "ORA H",
     .code = 0xB4,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ORA,
     .arg_a = REG_H},
    {.name = "ORA L",
     .code = 0xB5,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ORA,
     .arg_a = REG_L},
    {.name = "ORA M",
     .code = 0xB6,
     .bytes = 1,
     .clock_cycles = 7,
     .type = ORA,
     .arg_a = REG_M},
    {.name = "ORA A",
     .code = 0xB7,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ORA,
     .arg_a = REG_A},
    {.name = "CMP B",
     .code = 0xB8,
     .bytes = 1,
     .clock_cycles = 4,
     .type = CMP,
     .arg_a = REG_B},
    {.name = "CMP C",
     .code = 0xB9,
     .bytes = 1,
     .clock_cycles = 4,
     .type = CMP,
     .arg_a = REG_C},
    {.name = "CMP D",
     .code = 0xBA,
     .bytes = 1,
     .clock_cycles = 4,
     .type = CMP,
     .arg_a = REG_D},
    {.name = "CMP E",
     .code = 0xBB,
     .bytes = 1,
     .clock_cycles = 4,
     .type = CMP,
     .arg_a = REG_E},
    {.name = "CMP H",
     .code = 0xBC,
     .bytes = 1,
     .clock_cycles = 4,
     .type = CMP,
     .arg_a = REG_H},
    {.name = "CMP L",
     .code = 0xBD,
     .bytes = 1,
     .clock_cycles = 4,
     .type = CMP,
     .arg_a = REG_L},
    {.name = "CMP M",
     .code = 0xBE,
     .bytes = 1,
     .clock_cycles = 7,
     .type = CMP,
     .arg_a = REG_M},
    {.name = "CMP A",
     .code = 0xBF,
     .bytes = 1,
     .clock_cycles = 4,
     .type = CMP,
     .arg_a = REG_A},
    {.name = "RNZ", .code = 0xC0, .bytes = 1, .clock_cycles = 1, .type = RNZ},
    {.name = "POP B",
     .code = 0xC1,
     .bytes = 1,
     .clock_cycles = 1,
     .type = POP,
     .arg_a = REG_B},
    {.name = "JNZ a16",
     .code = 0xC2,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JNZ,
     .arg_a = A16},
    {.name = "JMP a16",
     .code = 0xC3,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JMP,
     .arg_a = A16},
    {.name = "CNZ a16",
     .code = 0xC4,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CNZ,
     .arg_a = A16},
    {.name = "PUSH B",
     .code = 0xC5,
     .bytes = 1,
     .clock_cycles = 1,
     .type = PUSH,
     .arg_a = REG_B},
    {.name = "ADI d8",
     .code = 0xC6,
     .bytes = 2,
     .clock_cycles = 7,
     .type = ADI,
     .arg_a = D8},
    {.name = "RST 0",
     .code = 0xC7,
     .bytes = 1,
     .clock_cycles = 1,
     .type = RST,
     .arg_a = 0},
    {.name = "RZ", .code = 0xC8, .bytes = 1, .clock_cycles = 1, .type = RZ},
    {.name = "RET", .code = 0xC9, .bytes = 1, .clock_cycles = 1, .type = RET},
    {.name = "JZ a16",
     .code = 0xCA,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JZ,
     .arg_a = A16},
    {.name = "*JMP a16",
     .code = 0xCB,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JMP,
     .arg_a = A16},
    {.name = "CZ a16",
     .code = 0xCC,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CZ,
     .arg_a = A16},
    {.name = "CALL a16",
     .code = 0xCD,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CALL,
     .arg_a = A16},
    {.name = "ACI d8",
     .code = 0xCE,
     .bytes = 2,
     .clock_cycles = 7,
     .type = ACI,
     .arg_a = D8},
    {.name = "RST 1",
     .code = 0xCF,
     .bytes = 1,
     .clock_cycles = 1,
     .type = RST,
     .arg_a = 1},
    {.name = "RNC", .code = 0xD0, .bytes = 1, .clock_cycles = 1, .type = RNC},
    {.name = "POP D",
     .code = 0xD1,
     .bytes = 1,
     .clock_cycles = 1,
     .type = POP,
     .arg_a = REG_D},
    {.name = "JNC a16",
     .code = 0xD2,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JNC,
     .arg_a = A16},
    {.name = "OUT d8",
     .code = 0xD3,
     .bytes = 2,
     .clock_cycles = 1,
     .type = OUT,
     .arg_a = D8},
    {.name = "CNC a16",
     .code = 0xD4,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CNC,
     .arg_a = A16},
    {.name = "PUSH D",
     .code = 0xD5,
     .bytes = 1,
     .clock_cycles = 1,
     .type = PUSH,
     .arg_a = REG_D},
    {.name = "SUI d8",
     .code = 0xD6,
     .bytes = 2,
     .clock_cycles = 7,
     .type = SUI,
     .arg_a = D8},
    {.name = "RST 2",
     .code = 0xD7,
     .bytes = 1,
     .clock_cycles = 1,
     .type = RST,
     .arg_a = 2},
    {.name = "RC", .code = 0xD8, .bytes = 1, .clock_cycles = 1, .type = RC},
    {.name = "*RET", .code = 0xD9, .bytes = 1, .clock_cycles = 1, .type = RET},
    {.name = "JC a16",
     .code = 0xDA,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JC,
     .arg_a = A16},
    {.name = "IN d8",
     .code = 0xDB,
     .bytes = 2,
     .clock_cycles = 1,
     .type = IN,
     .arg_a = D8},
    {.name = "CC a16",
     .code = 0xDC,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CC,
     .arg_a = A16},
    {.name = "*CALL a16",
     .code = 0xDD,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CALL,
     .arg_a = A16},
    {.name = "SBI d8",
     .code = 0xDE,
     .bytes = 2,
     .clock_cycles = 7,
     .type = SBI,
     .arg_a = D8},
    {.name = "RST 3",
     .code = 0xDF,
     .bytes = 1,
     .clock_cycles = 1,
     .type = RST,
     .arg_a = 3},
    {.name = "RPO", .code = 0xE0, .bytes = 1, .clock_cycles = 1, .type = RPO},
    {.name = "POP H",
     .code = 0xE1,
     .bytes = 1,
     .clock_cycles = 1,
     .type = POP,
     .arg_a = REG_H},
    {.name = "JPO a16",
     .code = 0xE2,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JPO,
     .arg_a = A16},
    {.name = "XTHL", .code = 0xE3, .bytes = 1, .clock_cycles = 1, .type = XTHL},
    {.name = "CPO a16",
     .code = 0xE4,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CPO,
     .arg_a = A16},
    {.name = "PUSH H",
     .code = 0xE5,
     .bytes = 1,
     .clock_cycles = 1,
     .type = PUSH,
     .arg_a = REG_H},
    {.name = "ANI d8",
     .code = 0xE6,
     .bytes = 2,
     .clock_cycles = 7,
     .type = ANI,
     .arg_a = D8},
    {.name = "RST 4",
     .code = 0xE7,
     .bytes = 1,
     .clock_cycles = 1,
     .type = RST,
     .arg_a = 4},
    {.name = "RPE", .code = 0xE8, .bytes = 1, .clock_cycles = 1, .type = RPE},
    {.name = "PCHL", .code = 0xE9, .bytes = 1, .clock_cycles = 5, .type = PCHL},
    {.name = "JPE a16",
     .code = 0xEA,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JPE,
     .arg_a = A16},
    {.name = "XCHG", .code = 0xEB, .bytes = 1, .clock_cycles = 5, .type = XCHG},
    {.name = "CPE a16",
     .code = 0xEC,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CPE,
     .arg_a = A16},
    {.name = "*CALL a16",
     .code = 0xED,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CALL,
     .arg_a = A16},
    {.name = "XRI d8",
     .code = 0xEE,
     .bytes = 2,
     .clock_cycles = 7,
     .type = XRI,
     .arg_a = D8},
    {.name = "RST 5",
     .code = 0xEF,
     .bytes = 1,
     .clock_cycles = 1,
     .type = RST,
     .arg_a = 5},
    {.name = "RP", .code = 0xF0, .bytes = 1, .clock_cycles = 1, .type = RP},
    {.name = "POP PSW",
     .code = 0xF1,
     .bytes = 1,
     .clock_cycles = 1,
     .type = POP,
     .arg_a = PSW},
    {.name = "JP a16",
     .code = 0xF2,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JP,
     .arg_a = A16},
    {.name = "DI", .code = 0xF3, .bytes = 1, .clock_cycles = 4, .type = DI},
    {.name = "CP a16",
     .code = 0xF4,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CP,
     .arg_a = A16},
    {.name = "PUSH PSW",
     .code = 0xF5,
     .bytes = 1,
     .clock_cycles = 1,
     .type = PUSH,
     .arg_a = PSW},
    {.name = "ORI d8",
     .code = 0xF6,
     .bytes = 2,
     .clock_cycles = 7,
     .type = ORI,
     .arg_a = D8},
    {.name = "RST 6",
     .code = 0xF7,
     .bytes = 1,
     .clock_cycles = 1,
     .type = RST,
     .arg_a = 6},
    {.name = "RM", .code = 0xF8, .bytes = 1, .clock_cycles = 1, .type = RM},
    {.name = "SPHL", .code = 0xF9, .bytes = 1, .clock_cycles = 5, .type = SPHL},
    {.name = "JM a16",
     .code = 0xFA,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JM,
     .arg_a = A16},
    {.name = "EI", .code = 0xFB, .bytes = 1, .clock_cycles = 4, .type = EI},
    {.name = "CM a16",
     .code = 0xFC,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CM,
     .arg_a = A16},
    {.name = "*CALL a16",
     .code = 0xFD,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CALL,
     .arg_a = A16},
    {.name = "CPI d8",
     .code = 0xFE,
     .bytes = 2,
     .clock_cycles = 7,
     .type = CPI,
     .arg_a = D8},
    {.name = "RST 7",
     .code = 0xFF,
     .bytes = 1,
     .clock_cycles = 1,
     .type = RST,
     .arg_a = 7}};