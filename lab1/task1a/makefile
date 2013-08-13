

# Tool invocations
# Executable "1a" depends on the files 1a.o.
nottail: 1a.o
	gcc -m32 -g -Wall -o 1a 1a.o

# Depends on the source files
1a.o: 1a.c
	gcc -g -Wall -m32 -ansi -c -o 1a.o 1a.c 

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o 1a 
