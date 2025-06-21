#include "history.h"
#include <stdio.h>
#include <string.h>

static t_history history;

const char* history_help =
"history: history [argument]\n"
"   Display or manipulate the history list.\n"
"   Options:\n"
"   -c	clear the history list by deleting all of the entries\n"
"   -d offset	delete the history entry at position OFFSET. Negative\n"
"offsets count back from the end of the history list\n";

void initialize_history(void) {
    history.length = 0;
    history.capacity = 10;
    history.entries = malloc(history.capacity * sizeof(*history.entries));

    // When malloc fails
    if(history.entries == NULL) {
        perror("History memory allocation");
        exit(EXIT_FAILURE);
    }
}

void free_history(void) {
    for(int i = 0; i < history.length; i++) {
        free(history.entries[i]);
    }

   free(history.entries); 
}

void history_cmd(char** argv, int argc) {
    if(argc > 2) {
        printf("Wrong usage. Type history --help to see the usage.\n");
        return;
    }

    if(argv[1] == NULL) {
        for(int i = 0; i < history.length; i++) {
            printf("%d %s", i + 1, history.entries[i]);
        }
    }

    bool clear = false;
    bool help = false;

    for(int i = 0; argv[i] != NULL; i++) {
        if(strcmp(argv[i], H_CLEAR) == 0) {
            clear = true;
        }

        if(strcmp(argv[i], H_HELP) == 0) {
            help = true;
        }
    }

    if(clear) {
        printf("History cleared\n");
        history.length = 0;
    } else if(help) {
        printf("%s", history_help);
    }

}

void history_add(char* entry) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    history.entries[history.length] = malloc(H_MAX_INPUT * sizeof(char));

    int nread = snprintf(history.entries[history.length++], H_MAX_INPUT, "%d-%02d-%02d %02d:%02d:%02d %s", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, entry);

    if(nread < 0) {
        perror("snprintf");
        exit(EXIT_FAILURE);
    }
    
    if(history.length >= history.capacity) {
        char** new;
        history.capacity *= 2;
        new = realloc(history.entries, history.capacity * sizeof(*new));

        if(new == NULL) {
            perror("Realloc");
            exit(EXIT_FAILURE);
        }

        history.entries = new;
    }
}

