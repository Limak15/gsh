#ifndef __HISTORY_H__
#define __HISTORY_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define H_MAX_CAPACITY 256
#define H_MAX_INPUT 8192
#define H_CLEAR "-c"
#define H_HELP "--help"

typedef struct s_history {
    char** entries; 
    size_t length;
    size_t capacity;
} t_history;

extern const char* history_help;

void initialize_history(void);
void free_history(void);
void history_cmd(char** argv, int argc);
void history_add(char* entry);

#endif
