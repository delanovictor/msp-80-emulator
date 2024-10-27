GCCFLAGS=-Wall -Wextra -Wpedantic

all:
	gcc $(GCCFLAGS) assembler/main.c -o assembler/main
	gcc $(GCCFLAGS) emulator/main.c -o emulator/main

assemble-and-emulate: assembler emulator

emulator:
	emulator/main ./examples/$(input).hex

assembler:
	assembler/main ./examples/$(input).asm

.PHONY: emulator assembler run