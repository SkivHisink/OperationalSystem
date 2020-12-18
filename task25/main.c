#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <sys/wait.h>
#include <stdlib.h>

#define STANDART_SIZE 1024

void change_to_upper(char* buff, size_t n)
{
	for (size_t i = 0; i < n; ++i)
		buff[i] = toupper(buff[i]);
}

void mock_func(char* a, size_t b)
{
}

int redirect(int read_fd, int write_fd, void (*editor)(char*, size_t))
{
	char buffer[STANDART_SIZE] = { 0 };
	ssize_t count = 0;

	while ((count = read(read_fd, buffer, STANDART_SIZE)) == -1)
	{
		if (errno != EINTR)
		{
			perror("read(3)");
			return 3;
		}
	}
	editor(buffer, count);

	if (write(write_fd, buffer, count) == -1)
	{
		perror("write(3)");
		return 4;
	}
	return 0;
}

void close_pipes(int pipes_container[2])
{
	if (close(pipes_container[0]) == -1)
	{
		printf("Failed to close pipe[0]\n");
	}
	if (close(pipes_container[1]) == -1)
	{
		printf("Failed to close pipe[1]\n");
	}
}

int wait_for_child()
{
	int ch_stat;
	while (wait(&ch_stat) == -1)
	{
		if (errno != EINTR)
		{
			perror("wait(1)");
			return 5;
		}
	}
	return 0;
}

int commViaPipe()
{
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
		perror("Error in fork");
		close_pipes(pipes_container);
		return 1;
	}
	case 0:
	{
		int res = redirect(pipes_container[1], STDOUT_FILENO, change_to_upper);
		close_pipes(pipes_container);
		int status;
		pid_t ChildPid;

		do
		{
			ChildPid = waitpid(child, &status, 0);// wait for child process to change state
			if (ChildPid == -1)
			{
				perror("waitpid(3) error:");
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
		return res ? 1 : 0;
	}
	}
	if (redirect(STDIN_FILENO, pipes_container[0], mock_func))
	{
		close_pipes(pipes_container);
		return 6;
	}
	if (wait_for_child())
	{
		perror("Error while waiting);
		close_pipe(p_filedes);
		return 7;
	}
	close_pipes(pipes_container);
	return 0;
}

int main()
{
	return commViaPipe();
}
