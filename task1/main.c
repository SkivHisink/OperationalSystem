#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/resource.h>

#define MAXDIR 256

extern char **environ;

void make_group_owner();

void print_core_limit();
void print_dir();
void print_env();
void print_gu_ids();
void print_p_ids();
void print_ulimit();

void set_core_limit(char *arg);
void set_ulimit(char *arg);

void put_env(char *arg);

int main(int argc, char* argv[])
{
    int param;
    while ((param = getopt(argc, argv, "ispuU:cC:dvV:")) != -1) {
        switch (param) {
        case 'c':
            print_core_limit();
            break;
        case 'C':
            set_core_limit(optarg);
            break;
        case 'd':
            print_dir();
            break;
        case 'i':
            print_gu_ids();
            break;
        case 'p':
            print_p_ids();
            break;
        case 's':
            make_group_owner();
            break;
        case 'u':
            print_ulimit();
            break;
        case 'U':
            set_ulimit(optarg);
            break;
        case 'v':
            print_env();
            break;
        case 'V':
            put_env(optarg);
            break;
        default: 
            return 1;
        }
    }
    return 0;
}

void make_group_owner() {
    if (setpgid(0, 0))
        perror("setpgid");
}

void print_core_limit() {
    struct rlimit limit;
    if (getrlimit(RLIMIT_CORE, &limit) == 0) {
        printf("Core file size limit: %ld\n", limit.rlim_cur);
    } else {
        perror("getrlimit");
    }
}

void print_dir() {
    char buffer[MAXDIR];
    char *dir = getcwd(buffer, MAXDIR);
    if (dir) {
        printf("%s\n", dir);
    } else {
        perror("getcwd");
    }
}

void print_gu_ids() {
    printf("Real user ID: %d\n", getuid());
    printf("Effective user ID: %d\n", geteuid());
    printf("Real group ID: %d\n", getgid());
    printf("Effective group ID: %d\n", getegid());
}

void print_env() {
    for (char **env = environ; *env; ++env)
        printf("%s\n", *env);
}

void print_p_ids() {
    printf("Process ID: %d\n", getpid());
    printf("Parent process ID: %d\n", getppid());
    printf("Process group ID: %d\n", getpgrp());
}

void print_ulimit() {
    struct rlimit limit;
    if (getrlimit(RLIMIT_FSIZE, &limit) == 0) {
        if (limit.rlim_cur == RLIM_INFINITY) {
            printf("Ulimit: unlimited\n");
        } else {
            printf("Ulimit: %ld\n", limit.rlim_cur);
        }
    } else {
        perror("getrlimit");
    }
}

void set_core_limit(char *arg) {
    char *endptr = NULL;
    errno = 0;
    long newLimit = strtol(arg, &endptr, 10);
    if (errno != 0 || *endptr || newLimit < 0) {
        fprintf(stderr, "Invalid new core limit value: %s\n", arg);
        return;
    }

    struct rlimit limit;
    if (getrlimit(RLIMIT_CORE, &limit) == 0) {
        limit.rlim_cur = (rlim_t) newLimit;
        if (setrlimit(RLIMIT_CORE, &limit) != 0) {
            perror("setrlimit");
        }
    } else {
        perror("getrlimit");
    }
}

void set_ulimit(char *arg) {
    char *endptr = NULL;
    errno = 0;
    long newLimit = strtol(arg, &endptr, 10);
    if (errno != 0 || *endptr || newLimit < 0) {
        fprintf(stderr, "Invalid new limit value: %s\n", arg);
        return;
    }

    struct rlimit limit;
    if (getrlimit(RLIMIT_FSIZE, &limit) == 0) {
        limit.rlim_cur = (rlim_t) newLimit;
        if (setrlimit(RLIMIT_FSIZE, &limit) != 0) {
            perror("setrlimit");
        }
    } else {
        perror("getrlimit");
    }
}

void put_env(char *arg) {
    if (putenv(arg) != 0)
        perror("putenv");
}
