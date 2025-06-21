#ifndef __GSH_H__
#define __GSH_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/wait.h>
#include <errno.h>

#include "builtin/builtins.h"
#include "history.h"

#define COMMAND_DEL "\n\t \v\f\r"
#define CHILD_PID 0

typedef struct s_builtins {
    char* command;
    void (*func)(char**, int);
} t_builtins;

void gsh_run(int argc, char** argv);

#endif // __GSH_H__
