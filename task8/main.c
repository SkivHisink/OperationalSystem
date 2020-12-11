#include <sys/types.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

int defended_text_editor()
{
	int file_descriptor = open("file.txt", O_RDWR);

	if (file_descriptor == -1)
	{
		perror("open(2) error");
		return 1;
	}

	struct flock file_info;
	memset(&file_info, 0, sizeof(file_info));

	file_info.l_type = F_WRLCK;
	file_info.l_whence = SEEK_SET;
	file_info.l_start = 0;
	file_info.l_len = 0;

	int returned = fcntl(file_descriptor, F_GETLK, &file_info);
	if (returned == -1)
	{
		perror("fcntl(3) error");
		int close_returned = close(file_descriptor);
		if (close_returned == -1)
		{
			perror("close(1) error");
		}
		return 2;
	}
	if (file_info.l_type != F_UNLCK) {
		printf("File is already locked by pid=%d\n", file_info.l_pid);
		return 3;
	}
	file_info.l_type = F_WRLCK;
	returned = fcntl(file_descriptor, F_SETLK, &file_info);

	if (returned == -1)
	{
		perror("fcntl(..,F_SETLK,..) error");
		return 4;
	}
	int sysRes = system("nano file.txt");

	if (sysRes == -1)
	{
		perror("system(1) error");
		int close_returned = close(file_descriptor);
		if (close_returned == -1)
		{
			perror("close(1) error");
		}
		flock(file_descriptor, LOCK_UN);
		return 5;
	}
	return 0;
}

int main()
{
	int return_val = defended_text_editor();
	return return_val;
}
