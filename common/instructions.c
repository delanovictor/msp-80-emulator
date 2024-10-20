#include <stdint.h>

#define DEFAULT_FLAG_MASK 0b00000010

#define SIGN_FLAG_MASK 0b10000000
#define SIGN_FLAG_POS 7

#define ZERO_FLAG_MASK 0b01000000
#define ZERO_FLAG_POS 6

#define AC_FLAG_MASK 0b00010000
#define AC_FLAG_POS 4

#define PARITY_FLAG_MASK 0b00000100
#define PARITY_FLAG_POS 2

#define CARRY_FLAG_MASK 0b00000001
#define CARRY_FLAG_POS 0

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
  NONE,
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
  char *mnemonic;
  enum InstructionType type;
  enum InstructionArgument arg_a;
  enum InstructionArgument arg_b;
  uint8_t code;
  uint8_t bytes;
  uint8_t clock_cycles;
};

uint8_t instruction_flag_mask_table[] = {
    DEFAULT_FLAG_MASK, // NOP,
    DEFAULT_FLAG_MASK, // LXI,
    DEFAULT_FLAG_MASK, // STAX,
    DEFAULT_FLAG_MASK, // INX,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK, // INR,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK,                // DCR,
    DEFAULT_FLAG_MASK,                   // MVI,
    DEFAULT_FLAG_MASK | CARRY_FLAG_MASK, // RLC,
    DEFAULT_FLAG_MASK | CARRY_FLAG_MASK, // DAD,
    DEFAULT_FLAG_MASK,                   // LDAX,
    DEFAULT_FLAG_MASK,                   // DCX,
    DEFAULT_FLAG_MASK | CARRY_FLAG_MASK, // RRC,
    DEFAULT_FLAG_MASK | CARRY_FLAG_MASK, // RAL,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK | CARRY_FLAG_MASK, // DAA,
    DEFAULT_FLAG_MASK | CARRY_FLAG_MASK,    // RAR,
    DEFAULT_FLAG_MASK,                      // SHLD,
    DEFAULT_FLAG_MASK,                      // LHLD,
    DEFAULT_FLAG_MASK,                      // CMA,
    DEFAULT_FLAG_MASK,                      // STA,
    DEFAULT_FLAG_MASK | CARRY_FLAG_MASK,    // STC,
    DEFAULT_FLAG_MASK,                      // LDA,
    DEFAULT_FLAG_MASK | CARRY_FLAG_MASK,    // CMC,
    DEFAULT_FLAG_MASK,                      // MOV,
    DEFAULT_FLAG_MASK,                      // HLT,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK | CARRY_FLAG_MASK, // ADD,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK | CARRY_FLAG_MASK, // ADC,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK | CARRY_FLAG_MASK, // SUB,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK | CARRY_FLAG_MASK, // SBB,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK | CARRY_FLAG_MASK, // ANA,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK | CARRY_FLAG_MASK, // XRA,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK | CARRY_FLAG_MASK, // ORA,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK | CARRY_FLAG_MASK, // CMP,
    DEFAULT_FLAG_MASK,                      // RNZ,
    DEFAULT_FLAG_MASK,                      // POP ***,
    DEFAULT_FLAG_MASK,                      // JNZ,
    DEFAULT_FLAG_MASK,                      // CNZ,
    DEFAULT_FLAG_MASK,                      // PUSH,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK | CARRY_FLAG_MASK, // ADI,
    DEFAULT_FLAG_MASK,                      // RST,
    DEFAULT_FLAG_MASK,                      // RZ,
    DEFAULT_FLAG_MASK,                      // RET,
    DEFAULT_FLAG_MASK,                      // JZ,
    DEFAULT_FLAG_MASK,                      // JMP,
    DEFAULT_FLAG_MASK,                      // CZ,
    DEFAULT_FLAG_MASK,                      // CALL,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK | CARRY_FLAG_MASK, // ACI,
    DEFAULT_FLAG_MASK,                      // RNC,
    DEFAULT_FLAG_MASK,                      // JNC,
    DEFAULT_FLAG_MASK,                      // OUT,
    DEFAULT_FLAG_MASK,                      // CNC,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK | CARRY_FLAG_MASK, // SUI,
    DEFAULT_FLAG_MASK,                      // RC,
    DEFAULT_FLAG_MASK,                      // JC,
    DEFAULT_FLAG_MASK,                      // IN,
    DEFAULT_FLAG_MASK,                      // CC,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK | CARRY_FLAG_MASK, // SBI,
    DEFAULT_FLAG_MASK,                      // RPO,
    DEFAULT_FLAG_MASK,                      // JPO,
    DEFAULT_FLAG_MASK,                      // XTHL,
    DEFAULT_FLAG_MASK,                      // CPO,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK | CARRY_FLAG_MASK, // ANI,
    DEFAULT_FLAG_MASK,                      // RPE,
    DEFAULT_FLAG_MASK,                      // PCHL,
    DEFAULT_FLAG_MASK,                      // JPE,
    DEFAULT_FLAG_MASK,                      // XCHG,
    DEFAULT_FLAG_MASK,                      // CPE,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK | CARRY_FLAG_MASK, // XRI,
    DEFAULT_FLAG_MASK,                      // RP,
    DEFAULT_FLAG_MASK,                      // JP,
    DEFAULT_FLAG_MASK,                      // DI,
    DEFAULT_FLAG_MASK,                      // CP,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK | CARRY_FLAG_MASK, // ORI,
    DEFAULT_FLAG_MASK,                      // RM,
    DEFAULT_FLAG_MASK,                      // SPHL,
    DEFAULT_FLAG_MASK,                      // JM,
    DEFAULT_FLAG_MASK,                      // EI,
    DEFAULT_FLAG_MASK,                      // CM,
    DEFAULT_FLAG_MASK | SIGN_FLAG_MASK | ZERO_FLAG_MASK | AC_FLAG_MASK |
        PARITY_FLAG_MASK | CARRY_FLAG_MASK, // CPI,
};

struct Instruction instruction_table[] = {
    {.mnemonic = "NOP", .code = 0x0, .bytes = 1, .clock_cycles = 4, .type = NOP},
    {.mnemonic = "LXI B",
     .code = 0x1,
     .bytes = 3,
     .clock_cycles = 1,
     .type = LXI,
     .arg_a = REG_B,
     .arg_b = D16},
    {.mnemonic = "STAX B",
     .code = 0x2,
     .bytes = 1,
     .clock_cycles = 7,
     .type = STAX,
     .arg_a = REG_B},
    {.mnemonic = "INX B",
     .code = 0x3,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INX,
     .arg_a = REG_B},
    {.mnemonic = "INR B",
     .code = 0x4,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INR,
     .arg_a = REG_B},
    {.mnemonic = "DCR B",
     .code = 0x5,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCR,
     .arg_a = REG_B},
    {.mnemonic = "MVI B",
     .code = 0x6,
     .bytes = 2,
     .clock_cycles = 7,
     .type = MVI,
     .arg_a = REG_B,
     .arg_b = D8},
    {.mnemonic = "RLC", .code = 0x7, .bytes = 1, .clock_cycles = 4, .type = RLC},
    {.mnemonic = "*NOP", .code = 0x8, .bytes = 1, .clock_cycles = 4, .type = NOP},
    {.mnemonic = "DAD B",
     .code = 0x9,
     .bytes = 1,
     .clock_cycles = 1,
     .type = DAD,
     .arg_a = REG_B},
    {.mnemonic = "LDAX B",
     .code = 0xA,
     .bytes = 1,
     .clock_cycles = 7,
     .type = LDAX,
     .arg_a = REG_B},
    {.mnemonic = "DCX B",
     .code = 0xB,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCX,
     .arg_a = REG_B},
    {.mnemonic = "INR C",
     .code = 0xC,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INR,
     .arg_a = REG_C},
    {.mnemonic = "DCR C",
     .code = 0xD,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCR,
     .arg_a = REG_C},
    {.mnemonic = "MVI C",
     .code = 0xE,
     .bytes = 2,
     .clock_cycles = 7,
     .type = MVI,
     .arg_a = REG_C,
     .arg_b = D8},
    {.mnemonic = "RRC", .code = 0xF, .bytes = 1, .clock_cycles = 4, .type = RRC},
    {.mnemonic = "*NOP", .code = 0x10, .bytes = 1, .clock_cycles = 4, .type = NOP},
    {.mnemonic = "LXI D",
     .code = 0x11,
     .bytes = 3,
     .clock_cycles = 1,
     .type = LXI,
     .arg_a = REG_D,
     .arg_b = D16},
    {.mnemonic = "STAX D",
     .code = 0x12,
     .bytes = 1,
     .clock_cycles = 7,
     .type = STAX,
     .arg_a = REG_D},
    {.mnemonic = "INX D",
     .code = 0x13,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INX,
     .arg_a = REG_D},
    {.mnemonic = "INR D",
     .code = 0x14,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INR,
     .arg_a = REG_D},
    {.mnemonic = "DCR D",
     .code = 0x15,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCR,
     .arg_a = REG_D},
    {.mnemonic = "MVI D",
     .code = 0x16,
     .bytes = 2,
     .clock_cycles = 7,
     .type = MVI,
     .arg_a = REG_D,
     .arg_b = D8},
    {.mnemonic = "RAL", .code = 0x17, .bytes = 1, .clock_cycles = 4, .type = RAL},
    {.mnemonic = "*NOP", .code = 0x18, .bytes = 1, .clock_cycles = 4, .type = NOP},
    {.mnemonic = "DAD D",
     .code = 0x19,
     .bytes = 1,
     .clock_cycles = 1,
     .type = DAD,
     .arg_a = REG_D},
    {.mnemonic = "LDAX D",
     .code = 0x1A,
     .bytes = 1,
     .clock_cycles = 7,
     .type = LDAX,
     .arg_a = REG_D},
    {.mnemonic = "DCX D",
     .code = 0x1B,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCX,
     .arg_a = REG_D},
    {.mnemonic = "INR E",
     .code = 0x1C,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INR,
     .arg_a = REG_E},
    {.mnemonic = "DCR E",
     .code = 0x1D,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCR,
     .arg_a = REG_E},
    {.mnemonic = "MVI E",
     .code = 0x1E,
     .bytes = 2,
     .clock_cycles = 7,
     .type = MVI,
     .arg_a = REG_E,
     .arg_b = D8},
    {.mnemonic = "RAR", .code = 0x1F, .bytes = 1, .clock_cycles = 4, .type = RAR},
    {.mnemonic = "*NOP", .code = 0x20, .bytes = 1, .clock_cycles = 4, .type = NOP},
    {.mnemonic = "LXI H",
     .code = 0x21,
     .bytes = 3,
     .clock_cycles = 1,
     .type = LXI,
     .arg_a = REG_H,
     .arg_b = D16},
    {.mnemonic = "SHLD",
     .code = 0x22,
     .bytes = 3,
     .clock_cycles = 1,
     .type = SHLD,
     .arg_a = A16},
    {.mnemonic = "INX H",
     .code = 0x23,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INX,
     .arg_a = REG_H},
    {.mnemonic = "INR H",
     .code = 0x24,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INR,
     .arg_a = REG_H},
    {.mnemonic = "DCR H",
     .code = 0x25,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCR,
     .arg_a = REG_H},
    {.mnemonic = "MVI H",
     .code = 0x26,
     .bytes = 2,
     .clock_cycles = 7,
     .type = MVI,
     .arg_a = REG_H,
     .arg_b = D8},
    {.mnemonic = "DAA", .code = 0x27, .bytes = 1, .clock_cycles = 4, .type = DAA},
    {.mnemonic = "*NOP", .code = 0x28, .bytes = 1, .clock_cycles = 4, .type = NOP},
    {.mnemonic = "DAD H",
     .code = 0x29,
     .bytes = 1,
     .clock_cycles = 1,
     .type = DAD,
     .arg_a = REG_H},
    {.mnemonic = "LHLD",
     .code = 0x2A,
     .bytes = 3,
     .clock_cycles = 1,
     .type = LHLD,
     .arg_a = A16},
    {.mnemonic = "DCX H",
     .code = 0x2B,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCX,
     .arg_a = REG_H},
    {.mnemonic = "INR L",
     .code = 0x2C,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INR,
     .arg_a = REG_L},
    {.mnemonic = "DCR L",
     .code = 0x2D,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCR,
     .arg_a = REG_L},
    {.mnemonic = "MVI L",
     .code = 0x2E,
     .bytes = 2,
     .clock_cycles = 7,
     .type = MVI,
     .arg_a = REG_L,
     .arg_b = D8},
    {.mnemonic = "CMA", .code = 0x2F, .bytes = 1, .clock_cycles = 4, .type = CMA},
    {.mnemonic = "*NOP", .code = 0x30, .bytes = 1, .clock_cycles = 4, .type = NOP},
    {.mnemonic = "LXI SP",
     .code = 0x31,
     .bytes = 3,
     .clock_cycles = 1,
     .type = LXI,
     .arg_a = SP,
     .arg_b = D16},
    {.mnemonic = "STA",
     .code = 0x32,
     .bytes = 3,
     .clock_cycles = 1,
     .type = STA,
     .arg_a = A16},
    {.mnemonic = "INX SP",
     .code = 0x33,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INX,
     .arg_a = SP},
    {.mnemonic = "INR M",
     .code = 0x34,
     .bytes = 1,
     .clock_cycles = 1,
     .type = INR,
     .arg_a = REG_M},
    {.mnemonic = "DCR M",
     .code = 0x35,
     .bytes = 1,
     .clock_cycles = 1,
     .type = DCR,
     .arg_a = REG_M},
    {.mnemonic = "MVI M",
     .code = 0x36,
     .bytes = 2,
     .clock_cycles = 1,
     .type = MVI,
     .arg_a = REG_M,
     .arg_b = D8},
    {.mnemonic = "STC", .code = 0x37, .bytes = 1, .clock_cycles = 4, .type = STC},
    {.mnemonic = "*NOP", .code = 0x38, .bytes = 1, .clock_cycles = 4, .type = NOP},
    {.mnemonic = "DAD SP",
     .code = 0x39,
     .bytes = 1,
     .clock_cycles = 1,
     .type = DAD,
     .arg_a = SP},
    {.mnemonic = "LDA",
     .code = 0x3A,
     .bytes = 3,
     .clock_cycles = 1,
     .type = LDA,
     .arg_a = A16},
    {.mnemonic = "DCX SP",
     .code = 0x3B,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCX,
     .arg_a = SP},
    {.mnemonic = "INR A",
     .code = 0x3C,
     .bytes = 1,
     .clock_cycles = 5,
     .type = INR,
     .arg_a = REG_A},
    {.mnemonic = "DCR A",
     .code = 0x3D,
     .bytes = 1,
     .clock_cycles = 5,
     .type = DCR,
     .arg_a = REG_A},
    {.mnemonic = "MVI A",
     .code = 0x3E,
     .bytes = 2,
     .clock_cycles = 7,
     .type = MVI,
     .arg_a = REG_A,
     .arg_b = D8},
    {.mnemonic = "CMC", .code = 0x3F, .bytes = 1, .clock_cycles = 4, .type = CMC},
    {.mnemonic = "MOV B,B",
     .code = 0x40,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_B,
     .arg_b = REG_B},
    {.mnemonic = "MOV B,C",
     .code = 0x41,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_B,
     .arg_b = REG_C},
    {.mnemonic = "MOV B,D",
     .code = 0x42,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_B,
     .arg_b = REG_D},
    {.mnemonic = "MOV B,E",
     .code = 0x43,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_B,
     .arg_b = REG_E},
    {.mnemonic = "MOV B,H",
     .code = 0x44,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_B,
     .arg_b = REG_H},
    {.mnemonic = "MOV B,L",
     .code = 0x45,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_B,
     .arg_b = REG_L},
    {.mnemonic = "MOV B,M",
     .code = 0x46,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_B,
     .arg_b = REG_M},
    {.mnemonic = "MOV B,A",
     .code = 0x47,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_B,
     .arg_b = REG_A},
    {.mnemonic = "MOV C,B",
     .code = 0x48,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_C,
     .arg_b = REG_B},
    {.mnemonic = "MOV C,C",
     .code = 0x49,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_C,
     .arg_b = REG_C},
    {.mnemonic = "MOV C,D",
     .code = 0x4A,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_C,
     .arg_b = REG_D},
    {.mnemonic = "MOV C,E",
     .code = 0x4B,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_C,
     .arg_b = REG_E},
    {.mnemonic = "MOV C,H",
     .code = 0x4C,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_C,
     .arg_b = REG_H},
    {.mnemonic = "MOV C,L",
     .code = 0x4D,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_C,
     .arg_b = REG_L},
    {.mnemonic = "MOV C,M",
     .code = 0x4E,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_C,
     .arg_b = REG_M},
    {.mnemonic = "MOV C,A",
     .code = 0x4F,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_C,
     .arg_b = REG_A},
    {.mnemonic = "MOV D,B",
     .code = 0x50,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_D,
     .arg_b = REG_B},
    {.mnemonic = "MOV D,C",
     .code = 0x51,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_D,
     .arg_b = REG_C},
    {.mnemonic = "MOV D,D",
     .code = 0x52,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_D,
     .arg_b = REG_D},
    {.mnemonic = "MOV D,E",
     .code = 0x53,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_D,
     .arg_b = REG_E},
    {.mnemonic = "MOV D,H",
     .code = 0x54,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_D,
     .arg_b = REG_H},
    {.mnemonic = "MOV D,L",
     .code = 0x55,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_D,
     .arg_b = REG_L},
    {.mnemonic = "MOV D,M",
     .code = 0x56,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_D,
     .arg_b = REG_M},
    {.mnemonic = "MOV D,A",
     .code = 0x57,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_D,
     .arg_b = REG_A},
    {.mnemonic = "MOV E,B",
     .code = 0x58,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_E,
     .arg_b = REG_B},
    {.mnemonic = "MOV E,C",
     .code = 0x59,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_E,
     .arg_b = REG_C},
    {.mnemonic = "MOV E,D",
     .code = 0x5A,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_E,
     .arg_b = REG_D},
    {.mnemonic = "MOV E,E",
     .code = 0x5B,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_E,
     .arg_b = REG_E},
    {.mnemonic = "MOV E,H",
     .code = 0x5C,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_E,
     .arg_b = REG_H},
    {.mnemonic = "MOV E,L",
     .code = 0x5D,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_E,
     .arg_b = REG_L},
    {.mnemonic = "MOV E,M",
     .code = 0x5E,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_E,
     .arg_b = REG_M},
    {.mnemonic = "MOV E,A",
     .code = 0x5F,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_E,
     .arg_b = REG_A},
    {.mnemonic = "MOV H,B",
     .code = 0x60,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_H,
     .arg_b = REG_B},
    {.mnemonic = "MOV H,C",
     .code = 0x61,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_H,
     .arg_b = REG_C},
    {.mnemonic = "MOV H,D",
     .code = 0x62,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_H,
     .arg_b = REG_D},
    {.mnemonic = "MOV H,E",
     .code = 0x63,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_H,
     .arg_b = REG_E},
    {.mnemonic = "MOV H,H",
     .code = 0x64,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_H,
     .arg_b = REG_H},
    {.mnemonic = "MOV H,L",
     .code = 0x65,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_H,
     .arg_b = REG_L},
    {.mnemonic = "MOV H,M",
     .code = 0x66,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_H,
     .arg_b = REG_M},
    {.mnemonic = "MOV H,A",
     .code = 0x67,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_H,
     .arg_b = REG_A},
    {.mnemonic = "MOV L,B",
     .code = 0x68,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_L,
     .arg_b = REG_B},
    {.mnemonic = "MOV L,C",
     .code = 0x69,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_L,
     .arg_b = REG_C},
    {.mnemonic = "MOV L,D",
     .code = 0x6A,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_L,
     .arg_b = REG_D},
    {.mnemonic = "MOV L,E",
     .code = 0x6B,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_L,
     .arg_b = REG_E},
    {.mnemonic = "MOV L,H",
     .code = 0x6C,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_L,
     .arg_b = REG_H},
    {.mnemonic = "MOV L,L",
     .code = 0x6D,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_L,
     .arg_b = REG_L},
    {.mnemonic = "MOV L,M",
     .code = 0x6E,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_L,
     .arg_b = REG_M},
    {.mnemonic = "MOV L,A",
     .code = 0x6F,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_L,
     .arg_b = REG_A},
    {.mnemonic = "MOV M,B",
     .code = 0x70,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_M,
     .arg_b = REG_B},
    {.mnemonic = "MOV M,C",
     .code = 0x71,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_M,
     .arg_b = REG_C},
    {.mnemonic = "MOV M,D",
     .code = 0x72,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_M,
     .arg_b = REG_D},
    {.mnemonic = "MOV M,E",
     .code = 0x73,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_M,
     .arg_b = REG_E},
    {.mnemonic = "MOV M,H",
     .code = 0x74,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_M,
     .arg_b = REG_H},
    {.mnemonic = "MOV M,L",
     .code = 0x75,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_M,
     .arg_b = REG_L},
    {.mnemonic = "HLT", .code = 0x76, .bytes = 1, .clock_cycles = 7, .type = HLT},
    {.mnemonic = "MOV M,A",
     .code = 0x77,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_M,
     .arg_b = REG_A},
    {.mnemonic = "MOV A,B",
     .code = 0x78,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_A,
     .arg_b = REG_B},
    {.mnemonic = "MOV A,C",
     .code = 0x79,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_A,
     .arg_b = REG_C},
    {.mnemonic = "MOV A,D",
     .code = 0x7A,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_A,
     .arg_b = REG_D},
    {.mnemonic = "MOV A,E",
     .code = 0x7B,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_A,
     .arg_b = REG_E},
    {.mnemonic = "MOV A,H",
     .code = 0x7C,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_A,
     .arg_b = REG_H},
    {.mnemonic = "MOV A,L",
     .code = 0x7D,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_A,
     .arg_b = REG_L},
    {.mnemonic = "MOV A,M",
     .code = 0x7E,
     .bytes = 1,
     .clock_cycles = 7,
     .type = MOV,
     .arg_a = REG_A,
     .arg_b = REG_M},
    {.mnemonic = "MOV A,A",
     .code = 0x7F,
     .bytes = 1,
     .clock_cycles = 5,
     .type = MOV,
     .arg_a = REG_A,
     .arg_b = REG_A},
    {.mnemonic = "ADD B",
     .code = 0x80,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADD,
     .arg_a = REG_B},
    {.mnemonic = "ADD C",
     .code = 0x81,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADD,
     .arg_a = REG_C},
    {.mnemonic = "ADD D",
     .code = 0x82,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADD,
     .arg_a = REG_D},
    {.mnemonic = "ADD E",
     .code = 0x83,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADD,
     .arg_a = REG_E},
    {.mnemonic = "ADD H",
     .code = 0x84,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADD,
     .arg_a = REG_H},
    {.mnemonic = "ADD L",
     .code = 0x85,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADD,
     .arg_a = REG_L},
    {.mnemonic = "ADD M",
     .code = 0x86,
     .bytes = 1,
     .clock_cycles = 7,
     .type = ADD,
     .arg_a = REG_M},
    {.mnemonic = "ADD A",
     .code = 0x87,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADD,
     .arg_a = REG_A},
    {.mnemonic = "ADC B",
     .code = 0x88,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADC,
     .arg_a = REG_B},
    {.mnemonic = "ADC C",
     .code = 0x89,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADC,
     .arg_a = REG_C},
    {.mnemonic = "ADC D",
     .code = 0x8A,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADC,
     .arg_a = REG_D},
    {.mnemonic = "ADC E",
     .code = 0x8B,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADC,
     .arg_a = REG_E},
    {.mnemonic = "ADC H",
     .code = 0x8C,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADC,
     .arg_a = REG_H},
    {.mnemonic = "ADC L",
     .code = 0x8D,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADC,
     .arg_a = REG_L},
    {.mnemonic = "ADC M",
     .code = 0x8E,
     .bytes = 1,
     .clock_cycles = 7,
     .type = ADC,
     .arg_a = REG_M},
    {.mnemonic = "ADC A",
     .code = 0x8F,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ADC,
     .arg_a = REG_A},
    {.mnemonic = "SUB B",
     .code = 0x90,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SUB,
     .arg_a = REG_B},
    {.mnemonic = "SUB C",
     .code = 0x91,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SUB,
     .arg_a = REG_C},
    {.mnemonic = "SUB D",
     .code = 0x92,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SUB,
     .arg_a = REG_D},
    {.mnemonic = "SUB E",
     .code = 0x93,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SUB,
     .arg_a = REG_E},
    {.mnemonic = "SUB H",
     .code = 0x94,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SUB,
     .arg_a = REG_H},
    {.mnemonic = "SUB L",
     .code = 0x95,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SUB,
     .arg_a = REG_L},
    {.mnemonic = "SUB M",
     .code = 0x96,
     .bytes = 1,
     .clock_cycles = 7,
     .type = SUB,
     .arg_a = REG_M},
    {.mnemonic = "SUB A",
     .code = 0x97,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SUB,
     .arg_a = REG_A},
    {.mnemonic = "SBB B",
     .code = 0x98,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SBB,
     .arg_a = REG_B},
    {.mnemonic = "SBB C",
     .code = 0x99,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SBB,
     .arg_a = REG_C},
    {.mnemonic = "SBB D",
     .code = 0x9A,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SBB,
     .arg_a = REG_D},
    {.mnemonic = "SBB E",
     .code = 0x9B,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SBB,
     .arg_a = REG_E},
    {.mnemonic = "SBB H",
     .code = 0x9C,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SBB,
     .arg_a = REG_H},
    {.mnemonic = "SBB L",
     .code = 0x9D,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SBB,
     .arg_a = REG_L},
    {.mnemonic = "SBB M",
     .code = 0x9E,
     .bytes = 1,
     .clock_cycles = 7,
     .type = SBB,
     .arg_a = REG_M},
    {.mnemonic = "SBB A",
     .code = 0x9F,
     .bytes = 1,
     .clock_cycles = 4,
     .type = SBB,
     .arg_a = REG_A},
    {.mnemonic = "ANA B",
     .code = 0xA0,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ANA,
     .arg_a = REG_B},
    {.mnemonic = "ANA C",
     .code = 0xA1,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ANA,
     .arg_a = REG_C},
    {.mnemonic = "ANA D",
     .code = 0xA2,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ANA,
     .arg_a = REG_D},
    {.mnemonic = "ANA E",
     .code = 0xA3,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ANA,
     .arg_a = REG_E},
    {.mnemonic = "ANA H",
     .code = 0xA4,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ANA,
     .arg_a = REG_H},
    {.mnemonic = "ANA L",
     .code = 0xA5,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ANA,
     .arg_a = REG_L},
    {.mnemonic = "ANA M",
     .code = 0xA6,
     .bytes = 1,
     .clock_cycles = 7,
     .type = ANA,
     .arg_a = REG_M},
    {.mnemonic = "ANA A",
     .code = 0xA7,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ANA,
     .arg_a = REG_A},
    {.mnemonic = "XRA B",
     .code = 0xA8,
     .bytes = 1,
     .clock_cycles = 4,
     .type = XRA,
     .arg_a = REG_B},
    {.mnemonic = "XRA C",
     .code = 0xA9,
     .bytes = 1,
     .clock_cycles = 4,
     .type = XRA,
     .arg_a = REG_C},
    {.mnemonic = "XRA D",
     .code = 0xAA,
     .bytes = 1,
     .clock_cycles = 4,
     .type = XRA,
     .arg_a = REG_D},
    {.mnemonic = "XRA E",
     .code = 0xAB,
     .bytes = 1,
     .clock_cycles = 4,
     .type = XRA,
     .arg_a = REG_E},
    {.mnemonic = "XRA H",
     .code = 0xAC,
     .bytes = 1,
     .clock_cycles = 4,
     .type = XRA,
     .arg_a = REG_H},
    {.mnemonic = "XRA L",
     .code = 0xAD,
     .bytes = 1,
     .clock_cycles = 4,
     .type = XRA,
     .arg_a = REG_L},
    {.mnemonic = "XRA M",
     .code = 0xAE,
     .bytes = 1,
     .clock_cycles = 7,
     .type = XRA,
     .arg_a = REG_M},
    {.mnemonic = "XRA A",
     .code = 0xAF,
     .bytes = 1,
     .clock_cycles = 4,
     .type = XRA,
     .arg_a = REG_A},
    {.mnemonic = "ORA B",
     .code = 0xB0,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ORA,
     .arg_a = REG_B},
    {.mnemonic = "ORA C",
     .code = 0xB1,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ORA,
     .arg_a = REG_C},
    {.mnemonic = "ORA D",
     .code = 0xB2,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ORA,
     .arg_a = REG_D},
    {.mnemonic = "ORA E",
     .code = 0xB3,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ORA,
     .arg_a = REG_E},
    {.mnemonic = "ORA H",
     .code = 0xB4,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ORA,
     .arg_a = REG_H},
    {.mnemonic = "ORA L",
     .code = 0xB5,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ORA,
     .arg_a = REG_L},
    {.mnemonic = "ORA M",
     .code = 0xB6,
     .bytes = 1,
     .clock_cycles = 7,
     .type = ORA,
     .arg_a = REG_M},
    {.mnemonic = "ORA A",
     .code = 0xB7,
     .bytes = 1,
     .clock_cycles = 4,
     .type = ORA,
     .arg_a = REG_A},
    {.mnemonic = "CMP B",
     .code = 0xB8,
     .bytes = 1,
     .clock_cycles = 4,
     .type = CMP,
     .arg_a = REG_B},
    {.mnemonic = "CMP C",
     .code = 0xB9,
     .bytes = 1,
     .clock_cycles = 4,
     .type = CMP,
     .arg_a = REG_C},
    {.mnemonic = "CMP D",
     .code = 0xBA,
     .bytes = 1,
     .clock_cycles = 4,
     .type = CMP,
     .arg_a = REG_D},
    {.mnemonic = "CMP E",
     .code = 0xBB,
     .bytes = 1,
     .clock_cycles = 4,
     .type = CMP,
     .arg_a = REG_E},
    {.mnemonic = "CMP H",
     .code = 0xBC,
     .bytes = 1,
     .clock_cycles = 4,
     .type = CMP,
     .arg_a = REG_H},
    {.mnemonic = "CMP L",
     .code = 0xBD,
     .bytes = 1,
     .clock_cycles = 4,
     .type = CMP,
     .arg_a = REG_L},
    {.mnemonic = "CMP M",
     .code = 0xBE,
     .bytes = 1,
     .clock_cycles = 7,
     .type = CMP,
     .arg_a = REG_M},
    {.mnemonic = "CMP A",
     .code = 0xBF,
     .bytes = 1,
     .clock_cycles = 4,
     .type = CMP,
     .arg_a = REG_A},
    {.mnemonic = "RNZ", .code = 0xC0, .bytes = 1, .clock_cycles = 1, .type = RNZ},
    {.mnemonic = "POP B",
     .code = 0xC1,
     .bytes = 1,
     .clock_cycles = 1,
     .type = POP,
     .arg_a = REG_B},
    {.mnemonic = "JNZ",
     .code = 0xC2,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JNZ,
     .arg_a = A16},
    {.mnemonic = "JMP",
     .code = 0xC3,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JMP,
     .arg_a = A16},
    {.mnemonic = "CNZ",
     .code = 0xC4,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CNZ,
     .arg_a = A16},
    {.mnemonic = "PUSH B",
     .code = 0xC5,
     .bytes = 1,
     .clock_cycles = 1,
     .type = PUSH,
     .arg_a = REG_B},
    {.mnemonic = "ADI",
     .code = 0xC6,
     .bytes = 2,
     .clock_cycles = 7,
     .type = ADI,
     .arg_a = D8},
    {.mnemonic = "RST 0",
     .code = 0xC7,
     .bytes = 1,
     .clock_cycles = 1,
     .type = RST,
     .arg_a = 0},
    {.mnemonic = "RZ", .code = 0xC8, .bytes = 1, .clock_cycles = 1, .type = RZ},
    {.mnemonic = "RET", .code = 0xC9, .bytes = 1, .clock_cycles = 1, .type = RET},
    {.mnemonic = "JZ",
     .code = 0xCA,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JZ,
     .arg_a = A16},
    {.mnemonic = "*JMP",
     .code = 0xCB,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JMP,
     .arg_a = A16},
    {.mnemonic = "CZ",
     .code = 0xCC,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CZ,
     .arg_a = A16},
    {.mnemonic = "CALL",
     .code = 0xCD,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CALL,
     .arg_a = A16},
    {.mnemonic = "ACI",
     .code = 0xCE,
     .bytes = 2,
     .clock_cycles = 7,
     .type = ACI,
     .arg_a = D8},
    {.mnemonic = "RST 1",
     .code = 0xCF,
     .bytes = 1,
     .clock_cycles = 1,
     .type = RST,
     .arg_a = 1},
    {.mnemonic = "RNC", .code = 0xD0, .bytes = 1, .clock_cycles = 1, .type = RNC},
    {.mnemonic = "POP D",
     .code = 0xD1,
     .bytes = 1,
     .clock_cycles = 1,
     .type = POP,
     .arg_a = REG_D},
    {.mnemonic = "JNC",
     .code = 0xD2,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JNC,
     .arg_a = A16},
    {.mnemonic = "OUT",
     .code = 0xD3,
     .bytes = 2,
     .clock_cycles = 1,
     .type = OUT,
     .arg_a = D8},
    {.mnemonic = "CNC",
     .code = 0xD4,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CNC,
     .arg_a = A16},
    {.mnemonic = "PUSH D",
     .code = 0xD5,
     .bytes = 1,
     .clock_cycles = 1,
     .type = PUSH,
     .arg_a = REG_D},
    {.mnemonic = "SUI",
     .code = 0xD6,
     .bytes = 2,
     .clock_cycles = 7,
     .type = SUI,
     .arg_a = D8},
    {.mnemonic = "RST 2",
     .code = 0xD7,
     .bytes = 1,
     .clock_cycles = 1,
     .type = RST,
     .arg_a = 2},
    {.mnemonic = "RC", .code = 0xD8, .bytes = 1, .clock_cycles = 1, .type = RC},
    {.mnemonic = "*RET", .code = 0xD9, .bytes = 1, .clock_cycles = 1, .type = RET},
    {.mnemonic = "JC",
     .code = 0xDA,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JC,
     .arg_a = A16},
    {.mnemonic = "IN",
     .code = 0xDB,
     .bytes = 2,
     .clock_cycles = 1,
     .type = IN,
     .arg_a = D8},
    {.mnemonic = "CC",
     .code = 0xDC,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CC,
     .arg_a = A16},
    {.mnemonic = "*CALL",
     .code = 0xDD,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CALL,
     .arg_a = A16},
    {.mnemonic = "SBI",
     .code = 0xDE,
     .bytes = 2,
     .clock_cycles = 7,
     .type = SBI,
     .arg_a = D8},
    {.mnemonic = "RST 3",
     .code = 0xDF,
     .bytes = 1,
     .clock_cycles = 1,
     .type = RST,
     .arg_a = 3},
    {.mnemonic = "RPO", .code = 0xE0, .bytes = 1, .clock_cycles = 1, .type = RPO},
    {.mnemonic = "POP H",
     .code = 0xE1,
     .bytes = 1,
     .clock_cycles = 1,
     .type = POP,
     .arg_a = REG_H},
    {.mnemonic = "JPO",
     .code = 0xE2,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JPO,
     .arg_a = A16},
    {.mnemonic = "XTHL", .code = 0xE3, .bytes = 1, .clock_cycles = 1, .type = XTHL},
    {.mnemonic = "CPO",
     .code = 0xE4,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CPO,
     .arg_a = A16},
    {.mnemonic = "PUSH H",
     .code = 0xE5,
     .bytes = 1,
     .clock_cycles = 1,
     .type = PUSH,
     .arg_a = REG_H},
    {.mnemonic = "ANI",
     .code = 0xE6,
     .bytes = 2,
     .clock_cycles = 7,
     .type = ANI,
     .arg_a = D8},
    {.mnemonic = "RST 4",
     .code = 0xE7,
     .bytes = 1,
     .clock_cycles = 1,
     .type = RST,
     .arg_a = 4},
    {.mnemonic = "RPE", .code = 0xE8, .bytes = 1, .clock_cycles = 1, .type = RPE},
    {.mnemonic = "PCHL", .code = 0xE9, .bytes = 1, .clock_cycles = 5, .type = PCHL},
    {.mnemonic = "JPE",
     .code = 0xEA,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JPE,
     .arg_a = A16},
    {.mnemonic = "XCHG", .code = 0xEB, .bytes = 1, .clock_cycles = 5, .type = XCHG},
    {.mnemonic = "CPE",
     .code = 0xEC,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CPE,
     .arg_a = A16},
    {.mnemonic = "*CALL",
     .code = 0xED,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CALL,
     .arg_a = A16},
    {.mnemonic = "XRI",
     .code = 0xEE,
     .bytes = 2,
     .clock_cycles = 7,
     .type = XRI,
     .arg_a = D8},
    {.mnemonic = "RST 5",
     .code = 0xEF,
     .bytes = 1,
     .clock_cycles = 1,
     .type = RST,
     .arg_a = 5},
    {.mnemonic = "RP", .code = 0xF0, .bytes = 1, .clock_cycles = 1, .type = RP},
    {.mnemonic = "POP PSW",
     .code = 0xF1,
     .bytes = 1,
     .clock_cycles = 1,
     .type = POP,
     .arg_a = PSW},
    {.mnemonic = "JP",
     .code = 0xF2,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JP,
     .arg_a = A16},
    {.mnemonic = "DI", .code = 0xF3, .bytes = 1, .clock_cycles = 4, .type = DI},
    {.mnemonic = "CP",
     .code = 0xF4,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CP,
     .arg_a = A16},
    {.mnemonic = "PUSH PSW",
     .code = 0xF5,
     .bytes = 1,
     .clock_cycles = 1,
     .type = PUSH,
     .arg_a = PSW},
    {.mnemonic = "ORI",
     .code = 0xF6,
     .bytes = 2,
     .clock_cycles = 7,
     .type = ORI,
     .arg_a = D8},
    {.mnemonic = "RST 6",
     .code = 0xF7,
     .bytes = 1,
     .clock_cycles = 1,
     .type = RST,
     .arg_a = 6},
    {.mnemonic = "RM", .code = 0xF8, .bytes = 1, .clock_cycles = 1, .type = RM},
    {.mnemonic = "SPHL", .code = 0xF9, .bytes = 1, .clock_cycles = 5, .type = SPHL},
    {.mnemonic = "JM",
     .code = 0xFA,
     .bytes = 3,
     .clock_cycles = 1,
     .type = JM,
     .arg_a = A16},
    {.mnemonic = "EI", .code = 0xFB, .bytes = 1, .clock_cycles = 4, .type = EI},
    {.mnemonic = "CM",
     .code = 0xFC,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CM,
     .arg_a = A16},
    {.mnemonic = "*CALL",
     .code = 0xFD,
     .bytes = 3,
     .clock_cycles = 1,
     .type = CALL,
     .arg_a = A16},
    {.mnemonic = "CPI",
     .code = 0xFE,
     .bytes = 2,
     .clock_cycles = 7,
     .type = CPI,
     .arg_a = D8},
    {.mnemonic = "RST 7",
     .code = 0xFF,
     .bytes = 1,
     .clock_cycles = 1,
     .type = RST,
     .arg_a = 7}};


#define NUMBER_OF_INSTRUCTIONS sizeof instruction_table / sizeof instruction_table[0]