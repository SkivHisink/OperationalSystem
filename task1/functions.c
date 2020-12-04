#include "functions.h"

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

voidset_ulimit(char *arg) {
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
