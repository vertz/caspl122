

# Tool invocations
# Executable "1c" depends on the files 1c.o.
nottail: 2.o
	gcc -m32 -g -Wall -o nottail 2.o

# Depends on the source files
2.o: 2.c
	gcc -g -Wall -m32 -ansi -c -o 2.o 2.c 

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o 2 
