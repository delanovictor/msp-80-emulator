#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>

// struct Token {
//     char* input;
//     int output;
// }

// struct Token[] tokens = {
//     {.input: "MOV", .output 0x3E},
// }

char* tokens[] = {
    "MOV",
    "MVI",
    "ADD"
};

int main(int argsc, char** argsv){
    printf("Hello from Assembler\n");

    FILE* f = fopen("./input.asm", "r");

    char c; 
    char buffer[10];
    memset(buffer, '0', 10);
    int buffer_pos = 0;

    bool instruction_found = false;
    bool arg1_found = false;
    bool arg2_found = false;

    while((c = getc(f)) != -1){

        if(c == ""){
            printf("Espaço, ignorando...\n");
            continue;
        }

        if(c == '\n'){
            printf("New line, resetando flags...\n");
            instruction_found = false;
            arg1_found = false;
            arg2_found = false;
            memset(buffer, '0', 10);
            buffer_pos = 0;
            continue;
        }
        
        buffer[buffer_pos++] = c;

        if(!instruction_found){
            for(int j = 0; j < 3; j++){
                printf("%s = %s\n", buffer, tokens[j]);
                if(strncmp(buffer, tokens[j], 3) == 0){
                    printf("Iguais!\n");
                    buffer_pos = 0;
                    memset(buffer, '0', 10);
                    instruction_found = true;
                    break;
                }
            }

            continue;
        }

        if(!arg1_found){
            printf("%s\n", buffer);
        }
        printf("\n");
    }

    printf("\n");
    
    fclose(f);

    return 0;
}