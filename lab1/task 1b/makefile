

# Tool invocations
# Executable "1b" depends on the files 1a.o.
nottail: 1b.o
	gcc -m32 -g -Wall -o 1b 1b.o

# Depends on the source files
1b.o: 1b.c
	gcc -g -Wall -m32 -ansi -c -o 1b.o 1b.c 

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o 1b 
