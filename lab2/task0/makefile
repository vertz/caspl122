# Tool invocations
# Executable "count-words" depends on the files count-words.o .
count-words: count-words.o 
	gcc -m32 -g -Wall -o count-words count-words.o

# Depends on the source files
count-words.o: count-words.c
	gcc -g -Wall -m32 -ansi -c -o count-words.o count-words.c 

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o count-words