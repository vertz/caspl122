

# Tool invocations
# Executable "1c" depends on the files 1c.o.
nottail: 1c.o
	gcc -m32 -g -Wall -o 1c 1c.o

# Depends on the source files
1c.o: 1c.c
	gcc -g -Wall -m32 -ansi -c -o 1c.o 1c.c 

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o 1c 
