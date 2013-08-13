/*
string-related utility functions for lab4
http://www.cs.bgu.ac.il/~caspl122/Lab4/Tasks
*/

/*
Returns the length of str.
The length of a C string is determined by the terminating null-character: 
A C string is as long as the amount of characters between the beginning 
of the string and the terminating null character.
example: strlen("abc") return 3
*/
unsigned int strlen (const char *str) 
{
  int i = 0;
  while (str[i])
  {
	++i;
  }
  return i;
}

/* in a 32-bit system, the maximal value of an int is 2^31 - 1 or 2,147,483,648.
 * This means that 10 digits are enough + 1 char for the terminating '\0' + 1 char for '-'.
 * in case of xtoa, it is 8 hex digits + 2 for the "0x" prefix.
 */
#define BUFFER_SIZE 12
 
/* we define the buffer as a global variable so we can return 
 * a pointer into it: local variables are saved on the stack 
 * and should never be refrenced outside the function!
 */
char buffer[BUFFER_SIZE];

/* returns the decimal representation of a _signed_ integer */
char *itoa(int num)
{
	char* p = buffer+BUFFER_SIZE-1;
	int neg = num<0;
	
	if(neg)
	{
		num = -num;
	}
	
	*p='\0';
	do {
		*(--p) = '0' + num%10;
	} while(num/=10);
	
	if(neg) 
	{
		*(--p) = '-';
	}
	
	return p;
}

/* returns the hexadecimal representation of a _unsigned_ integer */
char *itoax(unsigned int num) 
{	
	char* p = buffer+BUFFER_SIZE-1;
	*p='\0';
	do {
		if(num%16<10)
		{
			*(--p) = '0' + num%16;
		}
		else
		{
			*(--p) = 'A' + num%16 - 10;
		}
	} while(num/=16);
	*(--p) = 'x';
	*(--p) = '0';
	return p;
}

/*
Parses the C string str interpreting its content as an unsigned decimal number.
*/
unsigned int atoi(const char *str) 
{	
	int n;
	for(n=0; *str; str++)
	{
		n = n*10 + *str - '0';
	}
	return n;
}

/*
Compares the C string str1 to the C string str2.
This function starts comparing the first character of each string. If they are 
equal to each other, it continues with the following pairs until the characters 
differ or until a terminating null-character is reached.
*/
int strcmp(const char *str1, const char *str2) 
{
	while(*str1==*str2 && *str1) {
		str1++;
		str2++;
	}
	return *str1 - *str2;
}

/*
Copies the C string pointed by source into the array pointed by destination, 
including the terminating null character.

To avoid overflows, the size of the array pointed by destination should be long 
enough to contain the same C string as source (including the terminating null character), 
and should not overlap in memory with source.
*/
char *strcpy(char *dest, const char *src)
{	
	int i;
	for (i=0; src[i]; ++i)
	{
	  dest[i] = src[i];
	}
    dest[i] = '\0';

	return dest;
}

/* 
Appends a copy of the source string to the destination string. The terminating null 
character in destination is overwritten by the first character of source, and a new 
null-character is appended at the end of the new string formed by the concatenation 
of both in destination.
*/
char *strcat (char *dest, const char *src) 
{
	int dest_len = strlen(dest);
	int i;
	
	for (i=0; src[i]; ++i)
	{
		dest[dest_len + i] = src[i];
	}
    dest[dest_len + i] = '\0';

	return dest;
}


