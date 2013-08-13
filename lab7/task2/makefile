
# Tool invocations
# Executable "hexeditplus" depends on the file main.c.
hexeditplus: main.c
	gcc  -g -m32  main.c -o hexeditplus

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o hexeditplus 
