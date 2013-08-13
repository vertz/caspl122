
# Tool invocations
linker: linker.o lib.o
	gcc -g -Wall -o linker linker.o lib.o

# Depends on the source and header files
linker.o: merge.c lib.h
	gcc -g -Wall -ansi -c merge.c -o linker.o

lib.o: lib.c lib.h
	gcc -g -Wall -ansi -c -o lib.o lib.c


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o linker
