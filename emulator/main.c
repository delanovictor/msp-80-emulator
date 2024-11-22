#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include "../common/instructions.c"

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


#define RAM_SIZE 256 * 258 

uint8_t ram[RAM_SIZE] = {0};

#define REGISTER_COUNT 9

struct Register{
    char* name;
    uint8_t value;
};

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

#define SIGN_FLAG_VALUE (((registers[REG_PSW].value & SIGN_FLAG_MASK) >> SIGN_FLAG_POS) & 0x1)
#define ZERO_FLAG_VALUE (((registers[REG_PSW].value & ZERO_FLAG_MASK) >> ZERO_FLAG_POS) & 0x1)
#define AC_FLAG_VALUE (((registers[REG_PSW].value & AC_FLAG_MASK) >> AC_FLAG_POS) & 0x1)
#define PARITY_FLAG_VALUE (((registers[REG_PSW].value & PARITY_FLAG_MASK) >> PARITY_FLAG_POS) & 0x1)
#define CARRY_FLAG_VALUE (((registers[REG_PSW].value & CARRY_FLAG_MASK) >> CARRY_FLAG_POS) & 0x1)

uint16_t pc = 0;
uint16_t sp = 0x5000;


void initialize_ram(FILE*);

//TODO: unify
void print_source_code_line(int);
void print_source_code();
void print_instruction(struct Instruction);
void print_registers();
void print_stack(uint16_t before, uint16_t after);
void print_flags();
void print_ram(uint16_t start, uint16_t length);
void set_flag_bit(uint8_t flag_mask, uint8_t flag_pos, uint8_t flag_value);

uint8_t ascii_to_hex(char c);

int main(int argc, char* argsv[]){
    char error_message[256];

    if(argc == 1){
        fprintf(stderr, "Missing input file");
        exit(EXIT_FAILURE);
    }

    char* input_file_path = argsv[1];

    FILE* input_file = fopen(input_file_path, "r");

    if(input_file == NULL){
        sprintf(error_message,"Error opening input file %s", input_file_path);
        perror(error_message);
        exit(EXIT_FAILURE);
    }

    initialize_ram(input_file);
    
    print_ram(0, 16);
    print_source_code();
    
    struct Instruction curr_instr = {0};

    uint8_t affected_register = 0;
    uint8_t affected_flags = 0;
    bool default_flag_update = true;
    bool increment_pc = true;

    int clk_cycles = 0;
    
    printf("EXECUTION: \n\n");

    while(1){
        print_source_code_line(pc);
        clk_cycles++;

        //DECODE
        uint8_t byte = ram[pc];
        curr_instr = instruction_table[byte];

        affected_register = registers[REG_A].value;

        affected_flags = instruction_flag_mask_table[curr_instr.type];

        default_flag_update = true;

        //Temp buffer used to perform operations. It helps perform the flag updates at the end of the execution.
        long aux_buffer = 0;

        //EXECUTE
        switch(curr_instr.type){

            case MVI:
                registers[curr_instr.arg_a].value = ram[pc + 1];
                affected_register = registers[curr_instr.arg_a].value;
            break;

            case MOV:
                registers[curr_instr.arg_a].value = registers[curr_instr.arg_b].value;
                affected_register = registers[curr_instr.arg_a].value;
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
                affected_register = registers[curr_instr.arg_a].value;
            break;

            case DCR:
                registers[curr_instr.arg_a].value --;
                affected_register = registers[curr_instr.arg_a].value;
            break;

            case CMA:
                registers[REG_A].value = ~registers[REG_A].value;
            break;

            case CMP:
                aux_buffer = registers[REG_A].value - registers[curr_instr.arg_a].value;

                affected_register = aux_buffer;
            break;

            case CPI:
                aux_buffer = registers[REG_A].value - ram[pc + 1];

                affected_register = aux_buffer;
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

                print_stack(4, 4);

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

            case HLT:
                goto end;
            break;

            default:
                printf("\nInstruction %s (%#2x) not implemented\n", curr_instr.mnemonic, curr_instr.code);
        }    

        if(default_flag_update){
            //UPDATE FLAGS
            if(affected_flags & SIGN_FLAG_MASK){
                uint8_t sign = (affected_register & 0x80) >> 7;

                set_flag_bit(SIGN_FLAG_MASK, SIGN_FLAG_POS, sign);
            }

            if(affected_flags & ZERO_FLAG_MASK){
                // printf("Afetou ZERO\n");
                set_flag_bit(ZERO_FLAG_MASK, ZERO_FLAG_POS, (affected_register == 0));
            }

            if(affected_flags & AC_FLAG_MASK){
                // printf("Afetou AC\n");
            }

            if(affected_flags & PARITY_FLAG_MASK){
                // printf("Afetou PARITY\n");
                uint8_t a = affected_register;
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
                uint8_t new_carry_flag_bit = (aux_buffer & 0x100) >> (8 - CARRY_FLAG_POS);
                
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
    
        print_registers();

        if(increment_pc){
            pc += curr_instr.bytes;
        }else{
            increment_pc = true;
        }
    }

end:
    // print_source_code();
    print_ram(0x3FFA, 0x4005);
    print_stack(4, 4);
    print_registers();
    printf("Clock Cycles: %d\n", clk_cycles);
    return 0;
}


void set_flag_bit(uint8_t flag_mask, uint8_t flag_pos, uint8_t value){
    if(value){
        registers[REG_PSW].value = (registers[REG_PSW].value & ~(flag_mask)) | 1 << flag_pos;
    }else{
        registers[REG_PSW].value = (registers[REG_PSW].value & ~(flag_mask)) | 0 << flag_pos;
    }
}


void print_instruction(struct Instruction inst){
    printf("%s\n", inst.mnemonic);
    printf("Code: %d / %0#2X\n", inst.code, inst.code);
    printf("Type: %d\n", inst.type);
   
    printf("Type: %d\n", inst.type);
    // if(inst.bytes > 1){
    //     printf("Arg A: %d\n", inst.arg_a);
    // }
    // if(inst.bytes > 2){
    //     printf("Arg B: %d\n", inst.arg_b);
    // }
}

void print_flags(){
    char flag_symbol[] = {'C', '1', 'P', '0', 'A' , '0', 'Z', 'S'};

    printf("Flags: ( ");
    for(int i = 7; i >= 0; i--){
        
        int mask = 1 << i;
        // printf("mask: %d\n", mask);

        int bit = (registers[REG_PSW].value & mask) >> i;
        // printf("bit: %d\n", bit);

        if(bit){
            printf("%c", flag_symbol[i]);
        }else{
            printf("_");
        }

        printf(" ");
    }
    printf(")\n");
}

void print_registers(){
    printf("\n=====================\n");

    for(int i = 0; i < REGISTER_COUNT; i++){
        printf("%s: %d ("BYTE_TO_BINARY_PATTERN")\n", registers[i].name, registers[i].value, BYTE_TO_BINARY(registers[i].value));
    }
    print_flags();

    printf("=====================\n");
}

void print_ram(uint16_t start, uint16_t length){
    printf("=================================\n");
    printf("RAM DUMP: \n");

    for(int i = start; i < length; i++){
        printf("%#04x: %#04x\n", i, ram[i]);
    }
    printf("=================================\n");

}

void print_stack(uint16_t before, uint16_t after){
    printf("=================================\n");
    printf("STACK DUMP: \n");

    for(int i = sp - before; i < sp + after + 1; i++){
      
        printf("%#04x: %#04x", i, ram[i]);

        if(i == sp){
            printf(" <= SP");
        }

        printf("\n");
    }
    printf("=================================\n");
}

void print_source_code(){
    printf("=================================\n");
    printf("SOURCE CODE:\n");
    for(int i = 0, arg_count = 0; i < 16; i++){
        if(arg_count == 0){
            struct Instruction inst = instruction_table[ram[i]];

            printf("\n%s", inst.mnemonic);
            arg_count = inst.bytes - 1;
            
            if(inst.code == 0x76){
                break;
            }
        }else{
            printf(" %2X", ram[i]);
            arg_count--;
        }
    }

    printf("\n=================================\n");
}

void print_source_code_line(int pc){
    struct Instruction inst = instruction_table[ram[pc]];
    if(pc == 0){
        printf(" 0x0 - %s ",  inst.mnemonic);
    }else{
        printf("%#4x - %s ", pc, inst.mnemonic);
    }


    int arg_count = inst.bytes - 1;
    for(int i = 0; i < arg_count; i++){
        int next_pc = pc + i + 1;
        printf("%2X", ram[next_pc]);
    }       

    printf("\n");
}

void initialize_ram(FILE* fp){
  
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
