#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#define STANDART_SIZE 256

int blank_counter()
{
	FILE* pipe;
	if ((pipe = popen("grep ^$ ./text | wc -l ", "r")) == NULL)// he popen() function opens a process by creating a pipe, forking, and invoking the shell. Since a pipe is by definition unidirectional, the type argument may specify only reading or writing, not both; the resulting stream is correspondingly read-only or write-only.
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
	
	int status = pclose(pipe);
	if (status == -1) 
  {
		perror("pclose");
		return 3;
	}
	else if (WEXITSTATUS(status)) 
  {
		printf("Low-order 8 bits of the exit status value from the child process %d\n", WEXITSTATUS(status)); //If WIFEXITED is true of status, this macro returns the low-order 8 bits of the exit status value from the child process. See Exit Status.
	}
	else if (WIFSIGNALED(status))
  {
		printf("Signal number of the signal that terminated the child process is %d\n", WTERMSIG(status));//If WIFSIGNALED is true of status, this macro returns the signal number of the signal that terminated the child process.
	}
	
	printf("Count of blank lines is %ld\n", strtol(stringNumber, &end_ptr, 10));
	return 0;
}

int main(int argc, char* argv[])
{
	int return_val = blank_counter();

	return return_val;
}
