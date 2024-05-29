#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "instructions.c"

#define RAM_SIZE 2048 
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

void initialize_ram();

//TODO: unify
void print_source_code_line(int);
void print_source_code();

void print_registers();
void print_ram(int length);

uint8_t ascii_to_hex(char c);

int main(int argc, char* argsv[]){

    initialize_ram();

    // print_ram(16);
 
    // print_source_code();
    
    int arg_count = 0;
    struct Instruction curr_instr = {0};

    while(1){
        print_source_code_line(pc);

        //DECODE
        uint8_t byte = ram[pc];
        curr_instr = instruction_table[byte];
        arg_count = curr_instr.bytes - 1;

        
        //EXECUTE
        switch(curr_instr.type){

            case MVI:
                registers[curr_instr.arg_a].value = ram[pc + 1];
            break;

            case MOV:
                registers[curr_instr.arg_a].value = registers[curr_instr.arg_b].value;
            break;

            case ADI:
                registers[REG_A].value += ram[pc + 1];
            break;

            case ADD:
                registers[REG_A].value += registers[curr_instr.arg_a].value;
            break;

            case SUI:
                registers[REG_A].value -= ram[pc + 1];
            break;

            case SUB:
                registers[REG_A].value -= registers[curr_instr.arg_a].value;
            break;

            case ANI:
                registers[REG_A].value &= ram[pc + 1];
            break;

            case ANA:
                registers[REG_A].value &= registers[curr_instr.arg_a].value;
            break;

            case ORI:
                registers[REG_A].value |= ram[pc + 1];
            break;

            case ORA:
                registers[REG_A].value |= registers[curr_instr.arg_a].value;
            break;

            case XRI:
                registers[REG_A].value ^= ram[pc + 1];
            break;

            case XRA:
                registers[REG_A].value ^= registers[curr_instr.arg_a].value;
            break;

            case INR:
                registers[curr_instr.arg_a].value ++;
            break;

            case DCR:
                registers[curr_instr.arg_a].value --;
            break;

            case NOP:

            break;

            case HLT:
                goto end;
            break;

            default:
                printf("\nInstruction %s (%#4x) not implemented\n", curr_instr.name, curr_instr.code);
        }            
            
        // print_registers();

        pc += curr_instr.bytes;
    }

end:
    // print_source_code();
    print_registers();
    return 0;
}

void print_registers(){
    printf("\n=====================\n");

    for(int i = 0; i < REGISTER_COUNT; i++){
        printf("%s: %d\n", registers[i].name, registers[i].value);
    }

    printf("=====================\n");
}

void print_ram(int length){
    for(int i = 0; i < length; i++){
        printf("%#04x: %#04x\n", i, ram[i]);
    }
}

void print_source_code(){
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
    FILE *fp = fopen("./input.txt", "r");

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

    if(c > 64 && c < 70){
        return c - 55;
    }

    return 0;
}
