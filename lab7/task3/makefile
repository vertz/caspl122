run : palindrome.o  
	gcc -g  palindrome.o  -ansi -m32  -Wall -o  run 

palindrome.o: palindrome.c
	gcc -g  -ansi -m32 -Wall -c palindrome.c -o palindrome.o

#tell make that "clean" is not a file name!
.PHONY : clean

#clean the build directory
clean : 
	rm run palindrome.o 