#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#define STANDART_SIZE 256

int blank_counter()
{
	FILE* pipe;
	if ((pipe = popen("grep ^$ file.txt | wc -l ", "r")) == NULL)// he popen() function opens a process by creating a pipe, forking, and invoking the shell. Since a pipe is by definition unidirectional, the type argument may specify only reading or writing, not both; the resulting stream is correspondingly read-only or write-only.
	{
		perror("popen(2):");
		return 1;
	}
	char buffer[STANDART_SIZE];
	char* stringNumber;
	char* end_ptr;
	if ((stringNumber = fgets(buffer, STANDART_SIZE, pipe)) == NULL && !feof(pipe))
	{
		perror("fgets");
		return 2;
	}
	if (pclose(pipe)) 
  	{
		perror("pclose");
		return 3;
	}
	printf("Count of blank lines is %ld\n", strtol(stringNumber, &end_ptr, 10));
	return 0;
}

int main(int argc, char* argv[])
{
	int return_val = blank_counter();

	return return_val;
}
