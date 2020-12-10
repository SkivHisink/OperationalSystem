#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
//program returns:
//1 - fork() error
//2 - execl(3) error
//3 - waitpid(3) error

int fork_prog()
{
	char* path = "file.txt";

	pid_t child = fork();//creating child process

	switch (child) {
	case -1:
		perror("fork() error:");
		return 1;
	case 0:
		int execl_status = execl("/bin/cat", "cat", path, (char*)NULL); //The exec() family of functions replaces the current process image with a new process image.
		if (execl_status == -1)
		{
			perror("execl(3) error:");
			return 2;
		}
		return 0;
	default:
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
			else if (WIFCONTINUED(status)) //Given status from a call to waitpid, return true if the child process was resumed by delivery of SIGCONT.
			{
				printf("Child process was resumed\n");
			}
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 0;
}

int main(int argc, char* argv[])
{
	int return_val = fork_prog();
	return return_val;
}
