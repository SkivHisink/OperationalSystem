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
void print_p_ids();
void print_ulimit();

void set_core_limit(char *arg);
void set_ulimit(char *arg);

void put_env(char *arg);
