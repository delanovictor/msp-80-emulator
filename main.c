#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>

#include "instructions.c"

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

int pc = 0;
int flags = 0b00000;

void initialize_ram();

//TODO: unify
void print_source_code_line(int);
void print_source_code();
void print_instruction(struct Instruction);
void print_registers();
void print_ram(int length);

uint8_t ascii_to_hex(char c);

int main(int argc, char* argsv[]){

    initialize_ram();
    
    print_ram(16);
    // print_source_code();
    
    int arg_count = 0;

    struct Instruction curr_instr = {0};
    int affected_flags = 0;
    int affected_register = 0;
    int skip_pc_incr = 0;
    int clk_cycles = 0;

    printf("\n\nStart: \n\n");

    while(1){
        print_source_code_line(pc);
        clk_cycles++;
        //DECODE
        uint8_t byte = ram[pc];
        // printf("%#4x\n", byte);
        curr_instr = instruction_table[byte];
        affected_flags = instruction_flag_mask_table[curr_instr.type];
        affected_register = REG_A;
        
        arg_count = curr_instr.bytes - 1;

        long temp = 0;

        //EXECUTE
        switch(curr_instr.type){

            case MVI:
                registers[curr_instr.arg_a].value = ram[pc + 1];
                affected_register = curr_instr.arg_a;
            break;

            case MOV:
                registers[curr_instr.arg_a].value = registers[curr_instr.arg_b].value;
                affected_register = curr_instr.arg_a;
            break;

            case ADI:
                temp = registers[REG_A].value + ram[pc + 1];
                registers[REG_A].value = temp;
            break;

            case ADD:
                temp = registers[REG_A].value + registers[curr_instr.arg_a].value;
                registers[REG_A].value = temp;
            break;

            case SUI:
                temp = registers[REG_A].value - ram[pc + 1];
                registers[REG_A].value = temp;
            break;

            case SUB:
                temp = registers[REG_A].value - registers[curr_instr.arg_a].value;
                registers[REG_A].value = temp;
            break;

            case ANI:
                temp = registers[REG_A].value & ram[pc + 1];
                registers[REG_A].value = temp;
            break;

            case ANA:
                temp = registers[REG_A].value & registers[curr_instr.arg_a].value;
                registers[REG_A].value = temp;
            break;

            case ORI:
                temp = registers[REG_A].value | ram[pc + 1];
                registers[REG_A].value = temp;
            break;

            case ORA:
                temp = registers[REG_A].value | registers[curr_instr.arg_a].value;
                registers[REG_A].value = temp;
            break;

            case XRI:
                temp = registers[REG_A].value ^ ram[pc + 1];
                registers[REG_A].value = temp;
            break;

            case XRA:
                temp = registers[REG_A].value ^ registers[curr_instr.arg_a].value;
                registers[REG_A].value = temp;  
            break;

            case INR:
                registers[curr_instr.arg_a].value ++;
                affected_register = curr_instr.arg_a;
            break;

            case DCR:
                registers[curr_instr.arg_a].value --;
                affected_register = curr_instr.arg_a;
            break;

            case CMA:
                registers[REG_A].value = ~registers[REG_A].value;
            break;

            case RAL:
                //MSB
                temp = (registers[REG_A].value & 0x80);
                //SHIFT
                registers[REG_A].value = registers[curr_instr.arg_a].value << 1;
                //CARRY MSB TO START
                registers[REG_A].value |= (temp >> 7);

                // registers[REG_A].value = temp;  
            break;

            case RAR:
                //LSB
                temp = (registers[REG_A].value & 0x01);
                //SHIFT
                registers[REG_A].value = registers[curr_instr.arg_a].value >> 1;
                //CARRY LSB TO END
                registers[REG_A].value |= (temp << 7);

                // registers[REG_A].value = temp;  
            break;

            case STA:
                temp = (ram[pc + 2] << 8) | ram[pc + 1] ;
                ram[temp] = registers[REG_A].value;
            break;

            case LDA:
                temp = (ram[pc + 2] << 8) | ram[pc + 1] ;
                registers[REG_A].value = ram[temp];
            break;

            case JMP:
                pc = ram[pc + 1];
                skip_pc_incr = 1;
            break;

            case JNZ:
                temp = (ZERO_FLAG_MASK & registers[REG_F].value) >> ZERO_FLAG_POS;
                
                if(!temp){
                    pc = ram[pc + 1];
                    skip_pc_incr = 1;
                }   

            break;

            case JZ:
                temp = (ZERO_FLAG_MASK & registers[REG_F].value) >> ZERO_FLAG_POS;
                
                if(temp){
                    pc = ram[pc + 1];
                    skip_pc_incr = 1;
                }
            break;

            case JNC:
                temp = (CARRY_FLAG_MASK & registers[REG_F].value) >> CARRY_FLAG_POS;
                
                if(!temp){
                    pc = ram[pc + 1];
                    skip_pc_incr = 1;
                }
            break;
            
            case JC:
                temp = (CARRY_FLAG_MASK & registers[REG_F].value) >> CARRY_FLAG_POS;
                
                if(temp){
                    pc = ram[pc + 1];
                    skip_pc_incr = 1;
                }
            break;
            
            case JP:
                temp = (SIGN_FLAG_MASK & registers[REG_F].value) >> SIGN_FLAG_POS;
                
                if(!temp){
                    pc = ram[pc + 1];
                    skip_pc_incr = 1;
                }
            break;

            case JM:
                temp = (SIGN_FLAG_MASK & registers[REG_F].value) >> SIGN_FLAG_POS;
                
                if(temp){
                    pc = ram[pc + 1];
                    skip_pc_incr = 1;
                }
            break;

            case JPO:
                temp = (PARITY_FLAG_MASK & registers[REG_F].value) >> PARITY_FLAG_POS;
                
                if(!temp){
                    pc = ram[pc + 1];
                    skip_pc_incr = 1;
                }
            break;

            case JPE:
                temp = (PARITY_FLAG_MASK & registers[REG_F].value) >> PARITY_FLAG_POS;
                
                if(temp){
                    pc = ram[pc + 1];
                    skip_pc_incr = 1;
                }
            break;

            case NOP:

            break;

            case HLT:
                goto end;
            break;

            default:
                printf("\nInstruction %s (%#4x) not implemented\n", curr_instr.name, curr_instr.code);
        }    

        //UPDATE FLAGS
        if(affected_flags & SIGN_FLAG_MASK){
            registers[REG_F].value |= (SIGN_FLAG_MASK & ((registers[affected_register].value & 0b1000000) ? 0xFF : 0x00) );
        }

        if(affected_flags & ZERO_FLAG_MASK){
            registers[REG_F].value |= (ZERO_FLAG_MASK & ((registers[affected_register].value == 0) ? 0xFF : 0x00) );
        }

        if(affected_flags & AC_FLAG_MASK){

        }

        if(affected_flags & PARITY_FLAG_MASK){
            uint8_t a = registers[affected_register].value;
            uint8_t count = 0;
            uint8_t lsb = 0;

            for(uint8_t i = 0; i < sizeof(a)* 8; i++){

                // printf("a: %d\n", a);
                lsb = 0x01 & a;
                count += lsb;

                // printf("lsb: %d\n", lsb);
                // printf("-------\n");

                a = a >> 1;
            }

            uint8_t parity = (count % 2 == 0);

            registers[REG_F].value |= (PARITY_FLAG_MASK & (parity ? 0xFF : 0x00) );
        }

        if(affected_flags & CARRY_FLAG_MASK){
            registers[REG_F].value |= (CARRY_FLAG_MASK & ((temp & 0x100) ? 0xFF : 0x00) );
        }
    
        // print_registers();

        if(skip_pc_incr == 1){
            skip_pc_incr = 0;
        }else{
            pc += curr_instr.bytes;
        }
    }

end:
    // print_source_code();
    print_registers();
    printf("Clock Cycles: %d\n", clk_cycles);
    return 0;
}

void print_instruction(struct Instruction inst){
    printf("%s\n", inst.name);
    printf("%d - %d\n", inst.code, inst.type);
}

void print_flags(){
    char flag_symbol[] = {'S', 'Z', '0', 'A', '0' , 'P', '1', 'C'};

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
    for(int i = 0; i < length; i++){
        printf("%#04x: %#04x\n", i, ram[i]);
    }
}

void print_source_code(){
    printf("=================================\n");

    for(int i = 0, arg_count = 0; i < 16; i++){
        if(arg_count == 0){
            struct Instruction inst = instruction_table[ram[i]];

            printf("\n%s", inst.name);
            arg_count = inst.bytes - 1;
            
            if(inst.code == 0x76){
                break;
            }
        }else{
            printf(" %#04x", ram[i]);
            arg_count--;
        }
    }

    printf("=================================\n");
}

void print_source_code_line(int pc){
    struct Instruction inst = instruction_table[ram[pc]];
    printf("%#4x - %s ", pc, inst.name);

    int arg_count = inst.bytes - 1;
    for(int i = 0; i < arg_count; i++){
        int next_pc = pc + i + 1;
        printf("%#4x",next_pc, ram[next_pc]);
    }       

    printf("\n");
}

void initialize_ram(){
    FILE *fp = fopen("./code_examples/fibonacci.hex", "r");

    if(!fp){
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
