#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int execvpe(const char* file, char* const argv[], char* const envp[]) {
    if (clearenv()) //The clearenv() function clears the environment area; no environment variables are defined immediately after the clearenv() call.
    {
        perror("clearenv() error:");
        return -1;
    }

    for (int i = 0; envp[i]; ++i) {
        if (putenv(envp[i])) //The putenv() function makes the value of the environment variable name equal to value by altering an existing variable or creating a new one. In either case, the string pointed to by string becomes part of the environment, so altering the string will change the environment.
        {
            perror("putenv() error:");
            return -1;
        }
    }
	if(execvp(file, argv))//Each of the functions in the exec family replaces the current process image with a new process image. The new image is constructed from a regular, executable file called the new process image file. This file is either an executable object file or a file of data for an interpreter. There is no return from a successful call to one of these functions because the calling process image is overlaid by the new process image.
	{
        perror("execvp(2) error:");
        return -1;
	}
    return 0;
}

int main(int argc, char* argv[]) {
    char* args[] = { "ARGS1", "ARGS2","ARGS3", NULL };
    char* envp[] = { "ENV1=wake", "ENV2=me", "ENV3=up:(", NULL };
    if (execvpe("printer.c", args, envp) == -1) 
    {
        perror("execvpe(3) error:");
        return -1;
    }
    return 0;
}
