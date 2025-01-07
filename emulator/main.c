#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include "../common/instructions.c"
#include "raylib.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"

#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 

#define RAM_SIZE 256 * 256
#define REGISTER_COUNT 9


struct Register{
    char* name;
    uint8_t value;
};

struct ExecutionResult {
    struct Register affected_register;
    bool register_overflow;
    bool default_flag_update;
    bool increment_pc;
};

#define SIGN_FLAG_VALUE (((registers[REG_PSW].value & SIGN_FLAG_MASK) >> SIGN_FLAG_POS) & 0x1)
#define ZERO_FLAG_VALUE (((registers[REG_PSW].value & ZERO_FLAG_MASK) >> ZERO_FLAG_POS) & 0x1)
#define AC_FLAG_VALUE (((registers[REG_PSW].value & AC_FLAG_MASK) >> AC_FLAG_POS) & 0x1)
#define PARITY_FLAG_VALUE (((registers[REG_PSW].value & PARITY_FLAG_MASK) >> PARITY_FLAG_POS) & 0x1)
#define CARRY_FLAG_VALUE (((registers[REG_PSW].value & CARRY_FLAG_MASK) >> CARRY_FLAG_POS) & 0x1)



// RAYLIB MODE


#define WINDOW_WIDHT 800
#define WINDOW_HEIGHT 800
#define WINDOW_PADDING 10

#define FONT_SIZE 25
#define TITLE_COLOR WHITE
#define TEXT_COLOR RAYWHITE
#define BACKGROUND_COLOR DARKGRAY

#define LINE_HEIGHT 30
#define LINE_Y_POS(line_number) (line_number * LINE_HEIGHT) + WINDOW_PADDING
#define COL_X_POS(col_number) (int)(col_number * (WINDOW_WIDHT / 3)) + WINDOW_PADDING;

#define CLOCK_INTERVAL 1

Font font;

void draw_registers();
void draw_ram(uint16_t start, uint16_t length);
void draw_stack(uint16_t start, uint16_t length);
void draw_source_code_line(uint16_t address);
void draw_source_code(uint8_t start, uint8_t length);
void draw_counters();
void draw_all();

void DrawTextB(const char *text, int posX, int posY, int fontSize, Color color);
Vector2 MeasureTextB(const char *text);

// LOGIC
void initialize_ram(char*);
void handle_clock_cycle();
struct ExecutionResult execute(struct Instruction curr_instr);
void update_flags(struct Instruction curr_instr, struct ExecutionResult execution_result);
void set_flag_bit(uint8_t flag_mask, uint8_t flag_pos, uint8_t flag_value);
void get_flag_string(char *text_buffer);
void clear_state();

uint8_t ascii_to_hex(char c);

// =============== GLOBAL STATE ===============
#define STACK_START 0x5000

uint16_t pc = 0;
uint16_t sp = STACK_START;
uint32_t clk_cycles = 0;
uint8_t ram[RAM_SIZE] = {0};

enum MachineMode {
    MANUAL,
    AUTO_RUN
};

enum MachineMode machine_mode = MANUAL;


enum MachineState {
    RUNNING,
    HALTED
};

enum MachineState machine_state = RUNNING;


struct Register registers[REGISTER_COUNT] = {
    {.name="A"},
    {.name="B"},
    {.name="C"},
    {.name="D"},
    {.name="E"},
    {.name="H"},
    {.name="L"},
    {.name="M"},
    {.name="PSW"},
};


int main(int argc, char* argsv[]){

    if(argc == 1){
        fprintf(stderr, "Missing input file");
        exit(EXIT_FAILURE);
    }

    char* input_file_path = argsv[1];

    initialize_ram(input_file_path);
    
    printf("EXECUTION: \n\n");

    InitWindow(WINDOW_WIDHT,WINDOW_HEIGHT,"MSP-80 EMULATOR");

    font = LoadFont("fonts/DroidSansMono.ttf");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    BeginDrawing();

        ClearBackground(BACKGROUND_COLOR);

        draw_all();
        
    EndDrawing();

    float last_clock_ts = 0;
    float clock_interval = 1.0;
    
    while(!WindowShouldClose()){
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------

        BeginDrawing();

    
        if(IsKeyPressed(KEY_Q)){
            printf("Quit!\n");
            break;
        }

        if(IsKeyPressed(KEY_R)){
      
            clear_state();

            initialize_ram(input_file_path);

            printf("Reset!\n");
        }

        if(IsKeyPressed(KEY_A)){
            printf("Toggle Auto Run!\n");

            if(machine_mode == AUTO_RUN){
                machine_mode = MANUAL;
            }else{
                machine_mode = AUTO_RUN;
            }
        }

        if(IsKeyPressed(KEY_W)){
            clock_interval -= (clock_interval * 0.1);
            printf("increase frequency: %f Hz\n", 1/clock_interval);
        }

        if(IsKeyPressed(KEY_S)){
            clock_interval += (clock_interval * 0.1);
            printf("decrease frequency: %f Hz\n", 1/clock_interval);
        }

        if(machine_state == RUNNING){
            if(IsKeyPressed(KEY_SPACE) && machine_mode == MANUAL){
                printf("Manual Clock!\n");
                handle_clock_cycle();
            }

            if(machine_mode == AUTO_RUN){

                last_clock_ts += GetFrameTime();
                if(last_clock_ts > clock_interval){
                    printf("Auto Clock!\n");
                    last_clock_ts = 0;
                    handle_clock_cycle();
                }

            }
        }
        
        
        ClearBackground(BACKGROUND_COLOR);
            
        draw_all();

        EndDrawing();

    }

    CloseWindow();

    return 0;
}

void handle_clock_cycle(){
    // FETCH & DECODE
    uint8_t byte = ram[pc];

    struct Instruction curr_instr = instruction_table[byte];

    // EXECUTE
    clk_cycles++;

    if(curr_instr.type == HLT){
        machine_state = HALTED;
        return;
    }

    struct ExecutionResult execution_result = execute(curr_instr);
    
    if(execution_result.default_flag_update){
        update_flags(curr_instr, execution_result);
    }

    if(execution_result.increment_pc){
        pc += curr_instr.bytes;
    }
}

struct ExecutionResult execute(struct Instruction curr_instr){

    struct Register affected_register = registers[REG_A];

    bool increment_pc = true;
    bool default_flag_update = true;
    
    //Temp buffer used to perform operations. It helps perform the flag updates at the end of the execution.
    long aux_buffer = 0;

    //EXECUTE
    switch(curr_instr.type){

        case MVI:
            registers[curr_instr.arg_a].value = ram[pc + 1];
            affected_register = registers[curr_instr.arg_a];
        break;

        case MOV:
            registers[curr_instr.arg_a].value = registers[curr_instr.arg_b].value;
            affected_register = registers[curr_instr.arg_a];
        break;

        case ADI:
            aux_buffer = registers[REG_A].value + ram[pc + 1];
            registers[REG_A].value = aux_buffer;
        break;

        case ACI:
            aux_buffer = registers[REG_A].value + (ram[pc + 1] + CARRY_FLAG_VALUE);

            registers[REG_A].value = aux_buffer;
        break;

        case ADD:
            aux_buffer = registers[REG_A].value + registers[curr_instr.arg_a].value;
            registers[REG_A].value = aux_buffer;
        break;

        case ADC:
            aux_buffer = registers[REG_A].value + registers[curr_instr.arg_a].value + CARRY_FLAG_VALUE;
            registers[REG_A].value = aux_buffer;
        break;

        case SUI:
            aux_buffer = registers[REG_A].value - ram[pc + 1];

            registers[REG_A].value = aux_buffer;
        break;

        case SBI:
            aux_buffer = registers[REG_A].value - (ram[pc + 1] + CARRY_FLAG_VALUE);

            registers[REG_A].value = aux_buffer;
        break;

        case SUB:
            aux_buffer = registers[REG_A].value - registers[curr_instr.arg_a].value;

            registers[REG_A].value = aux_buffer;
        break;

        case SBB:
            aux_buffer = registers[REG_A].value - (registers[curr_instr.arg_a].value + CARRY_FLAG_VALUE);

            registers[REG_A].value = aux_buffer;
        break;

        case ANI:
            aux_buffer = registers[REG_A].value & ram[pc + 1];
            registers[REG_A].value = aux_buffer;
        break;

        case ANA:
            aux_buffer = registers[REG_A].value & registers[curr_instr.arg_a].value;
            registers[REG_A].value = aux_buffer;
        break;

        case ORI:
            aux_buffer = registers[REG_A].value | ram[pc + 1];
            registers[REG_A].value = aux_buffer;
        break;

        case ORA:
            aux_buffer = registers[REG_A].value | registers[curr_instr.arg_a].value;
            registers[REG_A].value = aux_buffer;
        break;

        case XRI:
            aux_buffer = registers[REG_A].value ^ ram[pc + 1];
            registers[REG_A].value = aux_buffer;
        break;

        case XRA:
            aux_buffer = registers[REG_A].value ^ registers[curr_instr.arg_a].value;
            registers[REG_A].value = aux_buffer;  
        break;

        case INR:
            registers[curr_instr.arg_a].value ++;
            affected_register = registers[curr_instr.arg_a];
        break;

        case DCR:
            registers[curr_instr.arg_a].value --;
            affected_register = registers[curr_instr.arg_a];
        break;

        case CMA:
            registers[REG_A].value = ~registers[REG_A].value;
        break;

        case CMP:
            aux_buffer = registers[REG_A].value - registers[curr_instr.arg_a].value;

            affected_register.value = aux_buffer;
        break;

        case CPI:
            aux_buffer = registers[REG_A].value - ram[pc + 1];

            affected_register.value = aux_buffer;
        break;

        case RLC:
            //MSB
            aux_buffer = (registers[REG_A].value & 0x80);
            //SHIFT
            registers[REG_A].value = registers[REG_A].value << 1;
            //ROTATE MSB TO START
            registers[REG_A].value |= (aux_buffer >> 7);
            
            //COPY MSB TO CARRY
            default_flag_update = false;
            set_flag_bit(CARRY_FLAG_MASK, CARRY_FLAG_POS, aux_buffer >> 7);
        break;
        case RRC:
            //LSB
            aux_buffer = (registers[REG_A].value & 0x01);
            //SHIFT
            registers[REG_A].value = registers[REG_A].value >> 1;
            //ROTATE LSB TO END
            registers[REG_A].value |= (aux_buffer << 7);
            
            //COPY LSB TO CARRY
            default_flag_update = false;
            set_flag_bit(CARRY_FLAG_MASK, CARRY_FLAG_POS, aux_buffer);
        break;
        case RAL:
            //MSB
            aux_buffer = (registers[REG_A].value & 0x80);
            //SHIFT
            registers[REG_A].value = registers[REG_A].value << 1;
            //ROTATE CARRY TO START
            registers[REG_A].value |= CARRY_FLAG_VALUE;

            //COPY MSB TO CARRY
            default_flag_update = false;
            set_flag_bit(CARRY_FLAG_MASK, CARRY_FLAG_POS, aux_buffer >> 7);
        break;

        case RAR:
            //LSB
            aux_buffer = (registers[REG_A].value & 0x01);
            //SHIFT
            registers[REG_A].value = registers[REG_A].value >> 1;
            //ROTATE CARRY TO START
            registers[REG_A].value |= (CARRY_FLAG_VALUE << 7);
            
            //COPY LSB TO CARRY
            default_flag_update = false;
            set_flag_bit(CARRY_FLAG_MASK, CARRY_FLAG_POS, aux_buffer);
        break;

        case STA:
            aux_buffer = (ram[pc + 2] << 8) | ram[pc + 1] ;
            ram[aux_buffer] = registers[REG_A].value;
        break;

        case LDA:
            aux_buffer = (ram[pc + 2] << 8) | ram[pc + 1] ;
            registers[REG_A].value = ram[aux_buffer];
        break;

        case JMP:
            pc = ram[pc + 1];
            increment_pc = false;
        break;

        case JNZ:
            if(!ZERO_FLAG_VALUE){
                pc = ram[pc + 1];
                increment_pc = false;
            }   
        break;

        case JZ:
            if(ZERO_FLAG_VALUE){
                pc = ram[pc + 1];
                increment_pc = false;
            }
        break;

        case JNC:
            if(!CARRY_FLAG_VALUE){
                pc = ram[pc + 1];
                increment_pc = false;
            }
        break;
        
        case JC:
            if(CARRY_FLAG_VALUE){
                pc = ram[pc + 1];
                increment_pc = false;
            }
        break;
        
        case JP:
            if(!SIGN_FLAG_VALUE){
                pc = ram[pc + 1];
                increment_pc = false;
            }
        break;

        case JM:
            if(SIGN_FLAG_VALUE){
                pc = ram[pc + 1];
                increment_pc = false;
            }
        break;

        case JPO:
            if(!PARITY_FLAG_VALUE){
                pc = ram[pc + 1];
                increment_pc = false;
            }
        break;

        case JPE:
            aux_buffer = (PARITY_FLAG_MASK & registers[REG_PSW].value) >> PARITY_FLAG_POS;
            
            if(PARITY_FLAG_VALUE){
                pc = ram[pc + 1];
                increment_pc = false;
            }
        break;

        case PCHL:
            //Build address from register pair
            aux_buffer = (registers[REG_H].value << 8) | (registers[REG_L].value);

            //Jump to address
            pc = aux_buffer;
            increment_pc = false;
        break;

        case STC:
            default_flag_update = false;
            set_flag_bit(CARRY_FLAG_MASK, CARRY_FLAG_POS, 1);
            break;
        break;

        case CMC:
            default_flag_update = false;
            set_flag_bit(CARRY_FLAG_MASK, CARRY_FLAG_POS, !CARRY_FLAG_VALUE);
            break;
        break;

        case DAD:
            //Build data from register pair
            if(curr_instr.arg_a == SP){
                aux_buffer = sp;
            }else{
                aux_buffer = (registers[curr_instr.arg_a].value << 8) | (registers[curr_instr.arg_a + 1].value);
            }

            aux_buffer += ((registers[REG_H].value << 8) | (registers[REG_L].value));

            registers[REG_H].value = (aux_buffer & 0xFF00) >> 8;
            registers[REG_L].value = (aux_buffer & 0x00FF);

            default_flag_update = false;

            if(aux_buffer > 0xFFFF || aux_buffer < 0){
                set_flag_bit(CARRY_FLAG_MASK, CARRY_FLAG_POS, 1);
            }else{
                set_flag_bit(CARRY_FLAG_MASK, CARRY_FLAG_POS, 1);
            }
            
        break;

        case STAX:
            //Build address from register pair
            aux_buffer = (registers[curr_instr.arg_a].value << 8) | (registers[curr_instr.arg_a + 1].value);
            
            // printf("STAX Address: %ld - %4lx\n", temp_buffer, temp_buffer);

            //Store the accumator at the target address
            ram[aux_buffer] = registers[REG_A].value;
        break;

        case LDAX:
            //Build address from register pair
            aux_buffer = (registers[curr_instr.arg_a].value << 8) | (registers[curr_instr.arg_a + 1].value);
            
            // printf("LDAX Address: %ld - %4lx\n", temp_buffer, temp_buffer);

            //Store the accumator at the target address
            registers[REG_A].value = ram[aux_buffer];
        break;

        case INX:
            if(curr_instr.arg_a == SP){
                sp++;
            }else{
                registers[curr_instr.arg_a + 1].value++;

                if(registers[curr_instr.arg_a + 1].value == 0x00){
                    registers[curr_instr.arg_a].value++;
                }
            }
        break;

        case DCX:
            if(curr_instr.arg_a == SP){
                sp--;
            }else{
                registers[curr_instr.arg_a + 1].value--;

                if(registers[curr_instr.arg_a + 1].value == 0xFF){
                    registers[curr_instr.arg_a].value--;
                }
            }
        break;

        case LXI:
            if(curr_instr.arg_a == SP){
                sp = (ram[pc+1] << 8) | ram[pc+2];
            }else{
                registers[curr_instr.arg_a].value = ram[pc+1];
                registers[curr_instr.arg_a + 1].value = ram[pc+2];
            }
        break;

        case SHLD:
            //Build address from args
            aux_buffer = (ram[pc + 1] << 8) | (ram[pc+2]);
            ram[aux_buffer] = registers[REG_L].value;
            ram[aux_buffer + 1] = registers[REG_H].value;
        break;

        case LHLD:
            //Build address from args
            aux_buffer = (ram[pc + 1] << 8) | (ram[pc+2]);
            registers[REG_L].value = ram[aux_buffer];
            registers[REG_H].value = ram[aux_buffer + 1];
        break;

        case SPHL:
            //Build address from register pair
            aux_buffer = (registers[REG_H].value << 8) | (registers[REG_L].value);

            //Set stack pointer to address
            sp = aux_buffer;
        break;

        case XCHG:

            // Swap REG_E with REG_L
            aux_buffer = (registers[REG_L].value);
            
            registers[REG_L].value = registers[REG_E].value;

            registers[REG_E].value= aux_buffer;

            // Swap REG_D with REG_H
            aux_buffer = (registers[REG_H].value);
            
            registers[REG_H].value = registers[REG_D].value;

            registers[REG_D].value= aux_buffer;

        break;


        case XTHL:

            // Swap REG_L with ram[ram[sp]]
            aux_buffer = (registers[REG_L].value);
            
            registers[REG_L].value = ram[ram[sp]];

            ram[ram[sp]] = aux_buffer;

            // Swap REG_H with ram[ram[sp + 1]]
            aux_buffer = (registers[REG_H].value);
            
            registers[REG_H].value = ram[ram[sp + 1]];

            ram[ram[sp + 1]] = aux_buffer;

        break;

        case PUSH:
            if(curr_instr.arg_a == REG_PSW){
                ram[sp - 1] = registers[REG_A].value;
                ram[sp - 2] = registers[REG_PSW].value;
            }else{
                ram[sp - 1] = registers[curr_instr.arg_a].value;
                ram[sp - 2] = registers[curr_instr.arg_a + 1].value;
            }

            sp = sp - 2;

            // print_stack(4, 4);
        break;

        case POP:
            if(curr_instr.arg_a == REG_PSW){
                registers[REG_PSW].value = ram[sp];
                registers[REG_A].value = ram[sp + 1];
                default_flag_update = false;
            }else{
                registers[curr_instr.arg_a + 1].value = ram[sp];
                registers[curr_instr.arg_a].value = ram[sp + 1];
            }

            sp = sp + 2;

        break;

        case CZ:
            if(ZERO_FLAG_VALUE){
                goto call_instruction;
            }
        break;

        case CNZ:
            if(!ZERO_FLAG_VALUE){
                goto call_instruction;
            }
        break;

        case CC:
            if(CARRY_FLAG_VALUE){
                goto call_instruction;
            }
        break;

        case CNC:
            if(!CARRY_FLAG_VALUE){
                goto call_instruction;
            }
        break;

        case CPE:
            if(PARITY_FLAG_VALUE){
                goto call_instruction;
            }
        break;

        case CPO:
            if(!PARITY_FLAG_VALUE){
                goto call_instruction;
            }
        break;

        case CM:
            if(SIGN_FLAG_VALUE){
                goto call_instruction;
            }
        break;

        case CP:
            if(!SIGN_FLAG_VALUE){
                goto call_instruction;
            }
        break;

        case CALL:
        call_instruction:
            // Build address from args
            aux_buffer = (ram[pc + 1] << 8) | (ram[pc+2]);

            printf("Subroutine address: %ld\n", aux_buffer);
            // Push current pc to stack
            ram[sp - 1] = pc + curr_instr.bytes;
            sp--;
            increment_pc = false;

            // Jump to subroutine
            pc = aux_buffer;

        break;

        case RZ:
            if(ZERO_FLAG_VALUE){
                goto return_instruction;
            }
        break;

        case RNZ:
            if(!ZERO_FLAG_VALUE){
                goto return_instruction;
            }
        break;

        case RC:
            if(CARRY_FLAG_VALUE){
                goto return_instruction;
            }
        break;

        case RNC:
            if(!CARRY_FLAG_VALUE){
                goto return_instruction;
            }
        break;

        case RPE:
            if(PARITY_FLAG_VALUE){
                goto return_instruction;
            }
        break;

        case RPO:
            if(!PARITY_FLAG_VALUE){
                goto return_instruction;
            }
        break;

        case RM:
            if(SIGN_FLAG_VALUE){
                goto return_instruction;
            }
        break;

        case RP:
            if(!SIGN_FLAG_VALUE){
                goto return_instruction;
            }
        break;

    
        case RET:
        return_instruction:
            pc = ram[sp];
            
            sp++;

            increment_pc = false;

            printf("Returning to: %d\n", pc);
        break;

        case NOP:

        break;

        default:
            printf("\nInstruction %s (%#2x) not implemented\n", curr_instr.mnemonic, curr_instr.code);
    }    
    
    struct ExecutionResult execution_result = {
        .affected_register = affected_register,
        .increment_pc = increment_pc,
        .default_flag_update = default_flag_update,
        .register_overflow = (aux_buffer & 0x100)
    };

    return execution_result;
}

void clear_state(){
    pc = 0;
    sp = STACK_START;
    clk_cycles = 0;
    machine_state = RUNNING;
    machine_mode = MANUAL;

    for(uint8_t i = 0; i < REGISTER_COUNT; i++){
        registers[i].value = 0;
    }

    memset(ram, 0, sizeof(ram));
}

void update_flags(struct Instruction curr_instr, struct ExecutionResult execution_result){
    uint8_t affected_flags = 0;

    affected_flags = instruction_flag_mask_table[curr_instr.type];

    //UPDATE FLAGS
    if(affected_flags & SIGN_FLAG_MASK){
        uint8_t sign = (execution_result.affected_register.value & 0x80) >> 7;

        set_flag_bit(SIGN_FLAG_MASK, SIGN_FLAG_POS, sign);
    }

    if(affected_flags & ZERO_FLAG_MASK){
        // printf("Afetou ZERO\n");
        set_flag_bit(ZERO_FLAG_MASK, ZERO_FLAG_POS, (execution_result.affected_register.value == 0));
    }

    if(affected_flags & AC_FLAG_MASK){
        // printf("Afetou AC\n");
    }

    if(affected_flags & PARITY_FLAG_MASK){
        // printf("Afetou PARITY\n");
        uint8_t a = execution_result.affected_register.value;
        uint8_t count = 0;
        uint8_t lsb = 0;

        for(uint8_t i = 0; i < sizeof(a)* 8; i++){

            lsb = 0x01 & a;
            count += lsb;

            a = a >> 1;
        }

        uint8_t parity = (count % 2 == 0);

        set_flag_bit(PARITY_FLAG_MASK, PARITY_FLAG_POS, parity);
    }

    if(affected_flags & CARRY_FLAG_MASK){
        uint8_t new_carry_flag_bit = execution_result.register_overflow >> (8 - CARRY_FLAG_POS);
        
        if((curr_instr.type == SBB) | (curr_instr.type == SUB) | (curr_instr.type == SUI)){
            if(new_carry_flag_bit){
                new_carry_flag_bit = 0;
            }else{
                new_carry_flag_bit = 1;
            }
        }

        // if(curr_instr.type == CMP){
        //     printf("é um CMP\n");
        //     if((registers[REG_A].value & 0x80) != (registers[curr_instr.arg_a].value & 0x80)){
        //         printf("sinais diferentes\n");
        //         printf("bit era %d", new_carry_flag_bit);
        //         if(new_carry_flag_bit){
        //             new_carry_flag_bit = 0;
        //         }else{
        //             new_carry_flag_bit = 1;
        //         }
        //         printf(" virou %d\n", new_carry_flag_bit);
        //     }else{
        //         printf("sinais iguais\n");
        //     }
        // } 

        set_flag_bit(CARRY_FLAG_MASK, CARRY_FLAG_POS, new_carry_flag_bit);
    }
}

void set_flag_bit(uint8_t flag_mask, uint8_t flag_pos, uint8_t value){
    if(value){
        registers[REG_PSW].value = (registers[REG_PSW].value & ~(flag_mask)) | 1 << flag_pos;
    }else{
        registers[REG_PSW].value = (registers[REG_PSW].value & ~(flag_mask)) | 0 << flag_pos;
    }
}

void draw_all(){
    draw_registers();

    draw_ram(pc, pc + 10);

    draw_counters();

    draw_source_code(0, 16);

    draw_stack((sp - 4), 10);
}

void draw_registers(){
    int base_pos_x = COL_X_POS(2);
    int start_line = 3;

    DrawTextB("REGISTERS:", base_pos_x, LINE_Y_POS((start_line)), FONT_SIZE, TITLE_COLOR);

    char text_buffer[100] = "";

    for(int i = 0; i < REGISTER_COUNT; i++){
        int line = start_line + 1 + i;

        sprintf(text_buffer, "%s:", registers[i].name);

        DrawTextB(text_buffer, base_pos_x, LINE_Y_POS(line), FONT_SIZE, TEXT_COLOR);
   
        sprintf(text_buffer, "%d", registers[i].value);

        DrawTextB(text_buffer, base_pos_x + 60, LINE_Y_POS(line), FONT_SIZE, TEXT_COLOR);    

        sprintf(text_buffer, "("BYTE_TO_BINARY_PATTERN")\n",  BYTE_TO_BINARY(registers[i].value));

        DrawTextB(text_buffer, base_pos_x + 110, LINE_Y_POS(line), FONT_SIZE, TEXT_COLOR);    
    }
}

void draw_stack(uint16_t start, uint16_t length){
    int base_pos_x = COL_X_POS(2);

    char text_buffer[100] = "";
    
    int start_line = 14;

    int end = start + length;
    
    DrawTextB("STACK:", base_pos_x, LINE_Y_POS((start_line)), FONT_SIZE, TITLE_COLOR);

    for(int i = start, j = (start_line + 1); i < end; i++, j++){
        sprintf(text_buffer, "%04x:  %02x", i, ram[i]);

        DrawTextB(text_buffer, base_pos_x, LINE_Y_POS(j), FONT_SIZE, TEXT_COLOR);

        if(sp == i){
            DrawCircle(base_pos_x + 150, LINE_Y_POS(j) + 10, 5, TEXT_COLOR);
        }

        // DrawLine(10, LINE_Y_POS(j), 100, LINE_Y_POS(j), TEXT_COLOR);
    }
}

void draw_ram(uint16_t start, uint16_t length){
    int base_pos_x = COL_X_POS(1);

    char text_buffer[100] = "";
    
    int cursor_line = start;
    
    // if (start <= 0){
    //     start = 0;
    //     cursor_line = 0;
    // }

    int start_line = 3;
    // int end_line = start_line + length;

    DrawTextB("RAM:", base_pos_x, LINE_Y_POS((start_line)), FONT_SIZE, TITLE_COLOR);

    for(int i = start, j = start_line + 1; i < length; i++, j++){
        sprintf(text_buffer, "%04x: %02x", i, ram[i]);

        DrawTextB(text_buffer, base_pos_x, LINE_Y_POS(j), FONT_SIZE, TEXT_COLOR);

        // if(cursor_line == i){
        //     DrawCircle(base_pos_x + 130, LINE_Y_POS(j) + 10, 5, TEXT_COLOR);
        // }
        // DrawLine(10, LINE_Y_POS(j), 100, LINE_Y_POS(j), TEXT_COLOR);
    }

    // DrawLine(60, LINE_Y_POS(start_line), 60, LINE_Y_POS(end_line), TEXT_COLOR);

    // for(int i = start; i < length; i++){
    //     sprintf(text_buffer, "%d", registers[i].value);

    //     DrawTextB(text_buffer, WINDOW_WIDHT - 175, LINE_Y_POS(i), 20, TEXT_COLOR);    
    // }

    // for(int i = start; i < length; i++){
    //     sprintf(text_buffer, "("BYTE_TO_BINARY_PATTERN")\n",  BYTE_TO_BINARY(registers[i].value));

    //     DrawTextB(text_buffer, WINDOW_WIDHT - 125, LINE_Y_POS(i), 20, TEXT_COLOR);    
    // }
}

void draw_source_code(uint8_t start, uint8_t length){
    int base_pos_x = COL_X_POS(0);
    int start_line = 3;
    int end = start + length;

    DrawTextB("SOURCE CODE:", base_pos_x, LINE_Y_POS((start_line)), FONT_SIZE, TITLE_COLOR);

    char text_buffer[100] = "";
    
    for(int i = start, arg_count = 0, instruction_count = 0; instruction_count < end; i++){


        if(arg_count == 0){
            struct Instruction inst = instruction_table[ram[i]];

            instruction_count++;
            sprintf(text_buffer, "%04x: %s", i, inst.mnemonic);

            // printf("\n%s", inst.mnemonic);

            arg_count = inst.bytes - 1;
            
            if(pc == i){
                int pos_y = LINE_Y_POS((instruction_count + start_line));
                DrawCircle(base_pos_x + 230, pos_y + 10, 5, TEXT_COLOR);
            }

        }else{
            char arg_buffer[10] = "";

            sprintf(arg_buffer, " %2X", ram[i]);

            strncat(text_buffer, arg_buffer, strlen(arg_buffer));
 
            arg_count--;
        }

        if(arg_count == 0){
            int pos_y = LINE_Y_POS((instruction_count + start_line));
            DrawTextB(text_buffer, base_pos_x, pos_y, FONT_SIZE, TEXT_COLOR);
        }
    }
}

void draw_counters(){
    char text_buffer[100] = "";
    int offset_x = WINDOW_PADDING;
    int spacing = (int)(WINDOW_WIDHT / 5);

    // ======= Cycles =======
    sprintf(text_buffer, "Cycle: %d", clk_cycles);

    DrawTextB(text_buffer, offset_x, LINE_Y_POS(0), FONT_SIZE, TITLE_COLOR);
    
    // ======= PC =======

    offset_x += MeasureTextB(text_buffer).x + spacing;

    sprintf(text_buffer, "PC: %#04x", pc);
    
    DrawTextB(text_buffer, offset_x , LINE_Y_POS(0), FONT_SIZE, TITLE_COLOR);

    // ======= SP =======

    offset_x += MeasureTextB(text_buffer).x + spacing;

    sprintf(text_buffer, "SP: %#04x", sp);

    DrawTextB(text_buffer, offset_x, LINE_Y_POS(0), FONT_SIZE, TITLE_COLOR);

    // ======= Flags =======

    offset_x = WINDOW_PADDING;

    //Empty buffer
    strcpy(text_buffer, "");

    get_flag_string((char*)text_buffer);

    DrawTextB(text_buffer, offset_x, LINE_Y_POS(1), FONT_SIZE, TITLE_COLOR);
}

void get_flag_string(char *text_buffer){
    char *flag_symbol[] = {"C", "1", "P", "0", "A" , "0", "Z", "S"};

    strcat(text_buffer, "Flags: ( ");

    for(int i = 7; i >= 0; i--){
        
        int mask = 1 << i;
        // printf("mask: %d\n", mask);

        int bit = (registers[REG_PSW].value & mask) >> i;
        // printf("bit: %d\n", bit);

        if(bit){
            strncat(text_buffer, flag_symbol[i], 1);
        }else{
            strcat(text_buffer, "_");
        }
        strcat(text_buffer, " ");
    }
    strcat(text_buffer, ")");
}

void initialize_ram(char* input_file_path){
    char error_message[256];
  
    FILE* fp = fopen(input_file_path, "r");

    if(fp == NULL){
        sprintf(error_message,"Error opening input file %s", input_file_path);
        perror(error_message);
        exit(EXIT_FAILURE);
    }

    int ram_index = 0;
    int i = 0;
    uint8_t word = 0;
    char c;

    while(1){
        c = fgetc(fp);

        // printf("%c (%d)\n", c, c);

        if(c == '\n' || c == ' ' || c == EOF){
            // printf("%d, %#04x\n\n\n", word, word);
            ram[ram_index] = word;
            word = 0;
            ram_index++;
            i++;

            if(c == EOF){
                break;
            }

            continue;
        }

        int n = ascii_to_hex(c);

        if(i % 3 == 0){
            // printf("1 Num: %d, %#04x\n", n, n);
            word += 0x10 * n;
        }

        if(i % 3 == 1){
            // printf("2 Num: %d, %#04x\n", n, n);
            word += n;
        }

        i++;
    }

    fclose(fp);
}

uint8_t ascii_to_hex(char c){
    if(c > 47 && c < 58){
        return c - 48;
    }

    if(c > 64 && c < 71){
        return c - 55;
    }

    return 0;
}

void DrawTextB(const char *text, int posX, int posY, int fontSize, Color color)
{
    DrawTextEx(font, text, (Vector2){ posX, posY }, fontSize, 1, color);
}

Vector2 MeasureTextB(const char *text){
    return MeasureTextEx(font, text, FONT_SIZE, 1);
}