#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include "tokens.c"

// struct Token {
//     char* input;
//     int output;
// }

// struct Token[] tokens = {
//     {.input: "MOV", .output 0x3E},
// }

char* get_instruction_code(char* token){
    for(int i = 0; i < NUMBER_OF_TOKENS; i++){
        if(strncmp(token, TOKENS[i].token, strlen(token)) == 0){
            return TOKENS[i].code;
        }
    }

    printf("Token não existente: %s\n", token);

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
    
    char delimiters[] = ",\n";

    memset(input_buffer, '0', 256);

    while(fgets(input_buffer, sizeof input_buffer, f) != NULL){
        printf("\n-----\nnew line: %s\n", input_buffer);
        char* token = strtok(input_buffer, delimiters);

    
        while(token){
            char word[64] = "";
            printf("token 1: %s\n", token);
            strncat(word, token, strlen(token));
            printf("word 1: %s\n", word);

            if(strncmp(token, "MOV", 3) == 0){
                token = strtok(NULL, delimiters);
                printf("token 2: %s\n", token);
                strncat(word, ",", strlen(token));
                strncat(word, token, strlen(token));
            }

            if(get_instruction_code(word) != NULL){
                char* code = get_instruction_code(word);

                printf("+ instruction : %s\n", code);

                strncat(output_buffer, code, strlen(code));
            }else{
                // char hex[3] = {0}; 
                // format_hex(hex, word);
                // printf("+ number : %s\n", hex);

                strncat(output_buffer, word, strlen(word));
            }

            strncat(output_buffer, " " , strlen(token));

            token = strtok(NULL, delimiters);

        }

    }
    
    fclose(f);

    printf("OUTPUT: %s\n", output_buffer);
    return 0;
}