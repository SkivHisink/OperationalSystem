#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define STANDART_SIZE 100

void change_to_upper(char* buff, size_t n)
{
	for (size_t i = 0; i < n; ++i)
		buff[i] = toupper(buff[i]);
}

int comm_using_std_lib_funcs(FILE* input) 
{
	char buffer[STANDART_SIZE];
	size_t count = fread(buffer, sizeof(char), STANDART_SIZE, input);

	if (ferror(input) == -1)
	{
		perror("fread(3)");
		if (pclose(input) == -1)
		{
			perror("pclose(1)");
		}
		return 2;
	}
	if (pclose(input) == -1)
	{
		perror("pclose(1)");
		return 3;
	}
	change_to_upper(buffer, count);
	fwrite(buffer, count, 1, stdout);

	if (ferror(input) == -1)
	{
		perror("fwrite(3)");
		return 4;
	}
	return 0;
}

int main()
{
	FILE* input = popen("cat file.txt", "r");
	if (input == NULL)
	{
		perror("popen(2)");
		return 1;
	}
	return comm_using_std_lib_funcs(input);
}
