# Tool invocations
# Executable "task0" depends on the files task0.o .
task0: task0.o 
	gcc -g -Wall -o task0 task0.o

# Depends on the source files
task0.o: task0.c
	gcc -g -Wall -ansi -c -o task0.o task0.c 

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task0