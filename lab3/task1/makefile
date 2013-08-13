 
# Tool invocations
# Executable "task1" depends on the files task1.o .
task1: task1.o 
	gcc -g -Wall -o task1 task1.o

# Depends on the source files
task1.o: task1.c
	gcc -g -Wall -ansi -c -o task1.o task1.c 

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task1