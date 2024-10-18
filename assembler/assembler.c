#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include "tokens.c"

struct Instruction* get_instruction(char* token){
    for(int i = 0; i < NUMBER_OF_INSTRUCTIONS; i++){
        if(strlen(INSTRUCTIONS[i].mnemonic) != strlen(token)){
            continue;
        }

        if(strncmp(token, INSTRUCTIONS[i].mnemonic, strlen(INSTRUCTIONS[i].mnemonic)) == 0){
            return &INSTRUCTIONS[i];
        }
    }

    // printf("Instrução não existente: %s\n", token);

    return NULL;
};

char* format_hex(char* new_hex, char* old_hex){
    printf("strlen(old_hex) = %d\n", strlen(old_hex));
    if(strlen(old_hex) == 3){
        new_hex[0] = '0';
        new_hex[1] = old_hex[2];
    }else if(strlen(new_hex) == 4){
        new_hex[0] = old_hex[2];
        new_hex[1] = old_hex[3];
    }

    new_hex[2] = '\0';
}


int main(int argsc, char** argsv){
    printf("Hello from Assembler\n");

    FILE* f = fopen("./fibonacci.asm", "r");
    char input_buffer[256];
    char output_buffer[1024] = {0};
    
    char delimiters[] = " ,\n";

    memset(input_buffer, '0', 256);

    while(fgets(input_buffer, sizeof input_buffer, f) != NULL){
        // printf("\n-----\nnew line: %s\n", input_buffer);

        struct Instruction* instr = NULL;
        char* token = strtok(input_buffer, delimiters);
        char word[64] = "";
        strncat(word, token, strlen(token));

        instr = get_instruction(word);

        if(instr == NULL){
            token = strtok(NULL, delimiters);
            strncat(word, " ", 1);
            strncat(word, token, strlen(token));
            instr = get_instruction(word);
        }

        if(instr == NULL){
            token = strtok(NULL, delimiters);
            strncat(word, ",", 1);
            strncat(word, token, strlen(token));
            instr = get_instruction(word);
        }

        if(instr == NULL) {
            printf("Invalid token: %s\n", word);
            exit(EXIT_FAILURE);
        }


        strncat(output_buffer, instr->code, strlen(instr->code));

        if(instr->bytes == 2){
            token = strtok(NULL, delimiters);

            strncat(output_buffer, " ", 1);
            strncat(output_buffer, token, strlen(token));
        }


        if(instr->bytes == 3){
            token = strtok(NULL, delimiters);

            strncat(output_buffer, " ", 1);
            strncat(output_buffer, token, strlen(token));

            token = strtok(NULL, delimiters);

            strncat(output_buffer, " ", 1);

            if(token == NULL){
                strncat(output_buffer, "00", 2);
            }else{
                strncat(output_buffer, token, strlen(token));
            }
          
        }

        strncat(output_buffer, "\n", 1);

        // printf("args: %d\n", instr->args);
        // printf("bytes: %d\n", instr->bytes);
        // printf("code: %s\n", instr->code);
        // printf("mnemonic: %s\n", instr->mnemonic);
    }
    
    fclose(f);

    printf("OUTPUT:\n%s\n", output_buffer);
    return 0;
}