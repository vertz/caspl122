#format is
#target-name: target dependencies
#{-tab-}actions
COMPILER:=gcc
C_FLAGS:=-g -m32 -Wall -c
L_FLAGS:=-g -m32 -Wall
 
# All Targets
all: can_link

# Tool invocations
can_link: can_link.o
	$(COMPILER) $(L_FLAGS) can_link.o -o can_link 

# Depends on the source and header files
can_link.o : main.c 
	$(COMPILER) $(C_FLAGS) main.c -o can_link.o
 

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o can_link 


