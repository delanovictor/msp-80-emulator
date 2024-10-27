#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include "../common/instructions.c"

struct Instruction* get_instruction(char* token){
    for(int i = 0; i < NUMBER_OF_INSTRUCTIONS; i++){
        if(strlen(instruction_table[i].mnemonic) != strlen(token)){
            continue;
        }

        if(strncmp(token, instruction_table[i].mnemonic, strlen(instruction_table[i].mnemonic)) == 0){
            return &instruction_table[i];
        }
    }

    // printf("Instrução não existente: %s\n", token);

    return NULL;
}

void format_hex(int code, char* out){
    if(code < 16){
        sprintf(out, "0%X", code);
    }else{
        sprintf(out, "%X", code);
    }
}

int main(int argsc, char** argsv){
    char error_message[512] = "";
    bool format = false;

    if(argsc == 1){
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

    FILE* output_file = stdout;
    char output_file_path[256] = "";

    for(int i = 2; i < argsc; i++){
        if(strncmp(argsv[i],"-format", 7) == 0){
            format = true;
        }

        if(i > (argsc - 1)){
            continue;
        }

        if(strncmp(argsv[i], "-o", 2) == 0){
            strncpy(output_file_path, argsv[i + 1], 256);
            output_file = fopen(output_file_path, "w");
        };
    }

    if(output_file == NULL){
        sprintf(error_message,"Error opening output file %s", output_file_path);
        perror(error_message);
        exit(EXIT_FAILURE);
    }

    char input_buffer[256];
    
    char delimiters[] = " ,\n";

    memset(input_buffer, '0', 256);

    while(fgets(input_buffer, sizeof input_buffer, input_file) != NULL){
        // printf("\n-----\nnew line: %s\n", input_buffer);

        struct Instruction* instr = NULL;
        char* token = strtok(input_buffer, delimiters);
        char word[64] = "";
        strncat(word, token, strlen(token));

        instr = get_instruction(word);

        if(instr == NULL){
            token = strtok(NULL, delimiters);
            strcat(word, " ");
            strncat(word, token, strlen(token));
            instr = get_instruction(word);
        }

        if(instr == NULL){
            token = strtok(NULL, delimiters);
            strcat(word, ",");
            strncat(word, token, strlen(token));
            instr = get_instruction(word);
        }

        if(instr == NULL) {
            fprintf(stderr, "Invalid token: %s\n", token);
            exit(EXIT_FAILURE);
        }

        char char_code[3] = "";
        format_hex(instr->code, char_code);
        fprintf(output_file, "%s", char_code);

        if(instr->bytes == 2){
            token = strtok(NULL, delimiters);

            fprintf(output_file, " ");
            fprintf(output_file, "%s", token);
        }


        if(instr->bytes == 3){
            token = strtok(NULL, delimiters);

            fprintf(output_file, " ");
            fprintf(output_file, "%s", token);

            token = strtok(NULL, delimiters);

            fprintf(output_file, " ");

            if(token == NULL){
                fprintf(output_file, "00");
            }else{
                fprintf(output_file, "%s", token);
            }
          
        }

        if(format){
            fprintf(output_file, "\n");
        }else{
            fprintf(output_file, " ");
        }

        // printf("args: %d\n", instr->args);
        // printf("bytes: %d\n", instr->bytes);
        // printf("code: %s\n", instr->code);
        // printf("mnemonic: %s\n", instr->mnemonic);
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}