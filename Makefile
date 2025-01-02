GCCFLAGS=-Wall -Wextra -Wpedantic
RAYLIB_PATH=./raylib/raylib-5.5/src
all:
	gcc $(GCCFLAGS) assembler/main.c -o assembler/main
	gcc $(GCCFLAGS) emulator/main.c -I$(RAYLIB_PATH) -L$(RAYLIB_PATH) -lraylib -lm -o emulator/main

assemble-and-emulate: assembler emulator

emulator:
	emulator/main ./examples/$(input).hex

assembler:
	assembler/main ./examples/$(input).asm -o ./examples/$(input).hex

.PHONY: emulator assembler run