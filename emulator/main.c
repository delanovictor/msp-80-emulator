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
    {.name="F"},
    {.name="H"},
    {.name="L"},
    {.name="M"},
};

#define SIGN_FLAG_VALUE (((registers[REG_F].value & SIGN_FLAG_MASK) >> SIGN_FLAG_POS) & 0x1)
#define ZERO_FLAG_VALUE (((registers[REG_F].value & ZERO_FLAG_MASK) >> ZERO_FLAG_POS) & 0x1)
#define AC_FLAG_VALUE (((registers[REG_F].value & AC_FLAG_MASK) >> AC_FLAG_POS) & 0x1)
#define PARITY_FLAG_VALUE (((registers[REG_F].value & PARITY_FLAG_MASK) >> PARITY_FLAG_POS) & 0x1)
#define CARRY_FLAG_VALUE (((registers[REG_F].value & CARRY_FLAG_MASK) >> CARRY_FLAG_POS) & 0x1)

int pc = 0;

void initialize_ram(FILE*);

//TODO: unify
void print_source_code_line(int);
void print_source_code();
void print_instruction(struct Instruction);
void print_registers();
void print_ram(int length);

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
    
    print_ram(16);
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
        long temp_buffer = 0;

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
                temp_buffer = registers[REG_A].value + ram[pc + 1];
                registers[REG_A].value = temp_buffer;
            break;

            case ACI:
                temp_buffer = registers[REG_A].value + (ram[pc + 1] + CARRY_FLAG_VALUE);

                registers[REG_A].value = temp_buffer;
            break;

            case ADD:
                temp_buffer = registers[REG_A].value + registers[curr_instr.arg_a].value;
                registers[REG_A].value = temp_buffer;
            break;

            case ADC:
                temp_buffer = registers[REG_A].value + registers[curr_instr.arg_a].value + CARRY_FLAG_VALUE;
                registers[REG_A].value = temp_buffer;
            break;

            case SUI:
                temp_buffer = registers[REG_A].value - ram[pc + 1];

                registers[REG_A].value = temp_buffer;
            break;

            case SBI:
                temp_buffer = registers[REG_A].value - (ram[pc + 1] + CARRY_FLAG_VALUE);

                registers[REG_A].value = temp_buffer;
            break;

            case SUB:
                temp_buffer = registers[REG_A].value - registers[curr_instr.arg_a].value;

                registers[REG_A].value = temp_buffer;
            break;

            case SBB:
                temp_buffer = registers[REG_A].value - (registers[curr_instr.arg_a].value + CARRY_FLAG_VALUE);

                registers[REG_A].value = temp_buffer;
            break;

            case ANI:
                temp_buffer = registers[REG_A].value & ram[pc + 1];
                registers[REG_A].value = temp_buffer;
            break;

            case ANA:
                temp_buffer = registers[REG_A].value & registers[curr_instr.arg_a].value;
                registers[REG_A].value = temp_buffer;
            break;

            case ORI:
                temp_buffer = registers[REG_A].value | ram[pc + 1];
                registers[REG_A].value = temp_buffer;
            break;

            case ORA:
                temp_buffer = registers[REG_A].value | registers[curr_instr.arg_a].value;
                registers[REG_A].value = temp_buffer;
            break;

            case XRI:
                temp_buffer = registers[REG_A].value ^ ram[pc + 1];
                registers[REG_A].value = temp_buffer;
            break;

            case XRA:
                temp_buffer = registers[REG_A].value ^ registers[curr_instr.arg_a].value;
                registers[REG_A].value = temp_buffer;  
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
                temp_buffer = registers[REG_A].value - registers[curr_instr.arg_a].value;

                affected_register = temp_buffer;
            break;

            case CPI:
                temp_buffer = registers[REG_A].value - ram[pc + 1];

                affected_register = temp_buffer;
            break;

            case RLC:
                //MSB
                temp_buffer = (registers[REG_A].value & 0x80);
                //SHIFT
                registers[REG_A].value = registers[REG_A].value << 1;
                //ROTATE MSB TO START
                registers[REG_A].value |= (temp_buffer >> 7);
                
                //COPY MSB TO CARRY
                default_flag_update = false;
                
                registers[REG_F].value = (registers[REG_F].value & ~(CARRY_FLAG_MASK)) | (temp_buffer >> 7);

            break;
            case RRC:
                //LSB
                temp_buffer = (registers[REG_A].value & 0x01);
                //SHIFT
                registers[REG_A].value = registers[REG_A].value >> 1;
                //ROTATE LSB TO END
                registers[REG_A].value |= (temp_buffer << 7);
               
                //COPY LSB TO CARRY
                default_flag_update = false;
                
                registers[REG_F].value = (registers[REG_F].value & ~(CARRY_FLAG_MASK)) | temp_buffer;
            break;
            case RAL:
                //MSB
                temp_buffer = (registers[REG_A].value & 0x80);
                //SHIFT
                registers[REG_A].value = registers[REG_A].value << 1;
                //ROTATE CARRY TO START
                registers[REG_A].value |= CARRY_FLAG_VALUE;

                //COPY MSB TO CARRY
                default_flag_update = false;
                
                registers[REG_F].value = (registers[REG_F].value & ~(CARRY_FLAG_MASK)) | (temp_buffer >> 7);
            break;

            case RAR:
                //LSB
                temp_buffer = (registers[REG_A].value & 0x01);
                //SHIFT
                registers[REG_A].value = registers[REG_A].value >> 1;
                //ROTATE CARRY TO START
                registers[REG_A].value |= (CARRY_FLAG_VALUE << 7);
                
                //COPY LSB TO CARRY
                default_flag_update = false;
                
                registers[REG_F].value = (registers[REG_F].value & ~(CARRY_FLAG_MASK)) | temp_buffer;
            break;

            case STA:
                temp_buffer = (ram[pc + 2] << 8) | ram[pc + 1] ;
                ram[temp_buffer] = registers[REG_A].value;
            break;

            case LDA:
                temp_buffer = (ram[pc + 2] << 8) | ram[pc + 1] ;
                registers[REG_A].value = ram[temp_buffer];
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
                temp_buffer = (PARITY_FLAG_MASK & registers[REG_F].value) >> PARITY_FLAG_POS;
                
                if(PARITY_FLAG_VALUE){
                    pc = ram[pc + 1];
                    increment_pc = false;
                }
            break;

            case STC:
                default_flag_update = false;

                registers[REG_F].value = (registers[REG_F].value & ~(CARRY_FLAG_MASK)) | 1 << CARRY_FLAG_POS;

                break;
            break;
            case CMC:
                default_flag_update = false;

                if(CARRY_FLAG_VALUE){
                    registers[REG_F].value = (registers[REG_F].value & ~(CARRY_FLAG_MASK)) | 0 << CARRY_FLAG_POS;
                }else{
                    registers[REG_F].value = (registers[REG_F].value & ~(CARRY_FLAG_MASK)) | 1 << CARRY_FLAG_POS;
                }
                break;
            break;

            case NOP:

            break;

            case HLT:
                goto end;
            break;

            default:
                printf("\nInstruction %s (%#4x) not implemented\n", curr_instr.mnemonic, curr_instr.code);
        }    

        if(default_flag_update){
            //UPDATE FLAGS
            if(affected_flags & SIGN_FLAG_MASK){
                uint8_t sign = (affected_register & 0x80) >> 7;
                uint8_t new_sign_flag_bit = sign << SIGN_FLAG_POS;

                registers[REG_F].value = (registers[REG_F].value & ~(SIGN_FLAG_MASK)) | new_sign_flag_bit;
            }

            if(affected_flags & ZERO_FLAG_MASK){
                // printf("Afetou ZERO\n");
                uint8_t new_zero_flag_bit = (affected_register == 0) << ZERO_FLAG_POS;

                registers[REG_F].value = (registers[REG_F].value & ~(ZERO_FLAG_MASK)) | new_zero_flag_bit;
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

                uint8_t new_parity_flag_bit = parity << PARITY_FLAG_POS;

                registers[REG_F].value = (registers[REG_F].value & ~(PARITY_FLAG_MASK)) | new_parity_flag_bit;
            }

            if(affected_flags & CARRY_FLAG_MASK){
                uint8_t new_carry_flag_bit = (temp_buffer & 0x100) >> (8 - CARRY_FLAG_POS);
                
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
                
                registers[REG_F].value = (registers[REG_F].value & ~(CARRY_FLAG_MASK)) | new_carry_flag_bit;
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
    print_registers();
    printf("Clock Cycles: %d\n", clk_cycles);
    return 0;
}

void print_instruction(struct Instruction inst){
    printf("%s\n", inst.mnemonic);
    printf("Code: %d / %0#2X\n", inst.code, inst.code);
    printf("Type: %d\n", inst.type);
}

void print_flags(){
    char flag_symbol[] = {'C', '1', 'P', '0', 'A' , '0', 'Z', 'S'};

    printf("Flags: ( ");
    for(int i = 7; i >= 0; i--){
        
        int mask = 1 << i;
        // printf("mask: %d\n", mask);

        int bit = (registers[REG_F].value & mask) >> i;
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

void print_ram(int length){
    printf("=================================\n");
    printf("RAM DUMP: \n");

    for(int i = 0; i < length; i++){
        printf("%#04x: %#04x\n", i, ram[i]);
    }
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
