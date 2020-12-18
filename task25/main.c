#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#define STANDART_SIZE 256

void close_pipes(int pipes_container_container[2])
{
	if (close(pipes_container_container[0]) == -1)
	{
		printf("Failed to close pipe[0]\n");
	}
	if (close(pipes_container_container[1]) == -1)
	{
		printf("Failed to close pipe[1]\n");
	}
}
void change_to_upper(char* buff, size_t n)
{
	for (size_t i = 0; i < n; ++i)
		buff[i] = toupper(buff[i]);
}

int main(int argc, char* argv[])
{
	char buffer[STANDART_SIZE] = "";
	int count = 0;
	int pipes_container[2];
	if (pipe(pipes_container) == -1)
	{
		perror("pipe(1)");
		return -1;
	}
	pid_t child = fork();
	switch (child) {
	case -1:
	{
		perror("fork()");
		close_pipes(pipes_container);
		return 2;
	}
	case 0:
	{
		if (write(pipes_container[1], buffer, strlen(buffer) + 1) == -1)
		{
			perror("write(3)");
			close_pipes(pipes_container);
			return 4;
		}
		printf("hay2 %s\n", buffer);
		close_pipes(pipes_container);
		return 1;
	}
	default:
	{
		while ((count = read(pipes_container[0], buffer, STANDART_SIZE)) == -1)
		{
			if (errno != EINTR)
			{
				perror("read(3)");
				return 3;
			}
		}
		change_to_upper(buffer, count);
		printf("hay%s\n", buffer);
		close_pipes(pipes_container);
		int status;
		do
		{
			pid_t ChildPid = waitpid(child, &status, 0);
			if (ChildPid == -1)
			{
				perror("waitpid(3)");
				return 3;
			}
			if (WIFEXITED(status))//This macro returns a nonzero value if the child process terminated normally with exit or _exit.
			{
				printf("Low-order 8 bits of the exit status value from the child process %d\n", WEXITSTATUS(status)); //If WIFEXITED is true of status, this macro returns the low-order 8 bits of the exit status value from the child process. See Exit Status.
			}
			else if (WIFSIGNALED(status))//This macro returns a nonzero value if the child process terminated because it received a signal that was not handled. See Signal Handling.
			{
				printf("Signal number of the signal that terminated the child process is %d\n", WTERMSIG(status));//If WIFSIGNALED is true of status, this macro returns the signal number of the signal that terminated the child process.
			}
			else if (WIFSTOPPED(status))//This macro returns a nonzero value if the child process is stopped.
			{
				printf("Signal is stopped. Signal that caused the child process to stop is %d\n", WSTOPSIG(status));//If WIFSTOPPED is true of status, this macro returns the signal number of the signal that caused the child process to stop.
			}
			else if (WIFCONTINUED(status)) //Given status from a call to waitpid, return true if the child process was resumed by delivery of SIGCOUNT.
			{
				printf("Child process was resumed\n");
			}
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		break;
	}
	}
	return 0;
}
