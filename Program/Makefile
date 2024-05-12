# Определяем переменную компилятора и флаги
CC=gcc
CFLAGS=-m32
NASM=nasm
NASMFLAGS=-f elf32
METHOD=

all: program

chord: METHOD="-DCHORD"
chord: all

tangent: METHOD="-DTANGENT"
tangent: all
	
main.o: main.c
	$(CC) $(CFLAGS) $(METHOD) -c -o main.o main.c

functions.o: functions.asm
	$(NASM) $(NASMFLAGS) -o functions.o functions.asm

program: main.o functions.o
	$(CC) -o program main.o functions.o $(CFLAGS)

clean:
	rm -f *.o program

