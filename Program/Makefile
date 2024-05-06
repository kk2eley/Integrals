all: program

main.o: main.c
	gcc -m32 -c -o main.o main.c

functions.o: functions.asm
	nasm -f elf32 -o functions.o functions.asm

program: main.o functions.o
	gcc -o program main.o functions.o -m32
  
clean:
	rm *.o program

