 
# Tool invocations
# Executable "patch" depends on the files util.o , start.o and main.o.
patch: start.o main.o util.o
	ld -melf_i386 start.o main.o util.o -o patch

# Depends on the source and header files
main.o: main.c
	gcc -m32 -Wall -c -nostdlib -fno-stack-protector main.c -o main.o

util.o: util.c util.h
	gcc -m32 -Wall -c -nostdlib -fno-stack-protector util.c -o util.o
 
start.o: start.s
	nasm -f elf start.s -o start.o


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o patch 
