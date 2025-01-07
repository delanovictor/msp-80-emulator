#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include "../common/instructions.c"

//TODO: unify

// TERMINAL MODE
void print_source_code_line(int);
void print_source_code();
void print_instruction(struct Instruction);
void print_registers();
void print_stack(uint16_t before, uint16_t after);
void print_flags();
void print_ram(uint16_t start, uint16_t length);


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
    char text_buffer[100];
    
    get_flag_string(text_buffer);

    printf("%s\n", text_buffer);
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