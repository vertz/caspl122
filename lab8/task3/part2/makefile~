#format is
#target-name: target dependencies
#{-tab-}actions
COMPILER:=gcc
C_FLAGS:=-g -m32 -Wall -c
L_FLAGS:=-g -m32 -Wall
 
# All Targets
all: examine

# Tool invocations
examine: examine.o
	$(COMPILER) $(L_FLAGS) examine.o -o examine 

# Depends on the source and header files
examine.o : main.c 
	$(COMPILER) $(C_FLAGS) main.c -o examine.o
 

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o examine 


