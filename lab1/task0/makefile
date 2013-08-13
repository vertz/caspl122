 
#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: main

# Tool invocations
# Executable "main" depends on the files main.o numbers.o add.o.
main: main.o numbers.o add.o
	gcc -m32 -g -Wall -o main main.o numbers.o add.o

# Depends on the source files
main.o: main.c
	gcc -g -Wall -m32 -ansi -c -o main.o main.c 

numbers.o: numbers.c
	gcc -g -Wall -m32 -ansi -c -o numbers.o numbers.c 
 
add.o: add.s
	nasm -g -f elf -w+all -o add.o add.s


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o main