 
# Tool invocations
# Executable "myshell" depends on the files util.o , start.o and main.o.
myshell: main.o LineParser.o
	gcc -g main.o LineParser.o -o myshell

# Depends on the source and header files
main.o: main.c
	gcc  -Wall -c main.c -o main.o

LineParser.o: LineParser.c LineParser.h
	gcc -Wall -c LineParser.c -o LineParser.o


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o myshell