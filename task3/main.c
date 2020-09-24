#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int write_uid_and_euid()
{
	printf("The real user ID : %d\nThe effective user ID : %d\n",
	 getuid(), geteuid());
	FILE* file = fopen("file", "r");
	if(file==NULL)
	{
		perror("File did't open");
	}
	else
	{
	//fprintf(file, "%s %s\n", getuid(), geteuid());
	fclose(file);
	printf("File opened\n");
	return 0;
	}
}
int main()
{
	if(write_uid_and_euid()==0)
	{
	setuid(geteuid());
	write_uid_and_euid();
	}
	return 0;
}
