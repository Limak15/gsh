#include "cd.h"
#include <signal.h>

const char* cd_help =
"cd: cd [dir]\n\n"
"Change the current directory to DIR.  The default DIR is the value of the\n"
"HOME shell variable.\n";

static void change_dir(const char* path) {
    if(chdir(path) == -1) {
        perror(path);
    }
}

void gsh_cd(char** args, int argc) {
    if(argc > 2) {
        fprintf(stderr, "Invalid arguments. Type cd --help to see usage\n");
        return;
    }
    
    // just cd. go to home dir
    if(argc == 1) {
        const char* home = getenv("HOME");

        if(home == NULL) {
            fprintf(stderr, "cd: HOME not set\n");
            return;
        }

        change_dir(getenv("HOME")); 
        return;
    }

    if(strcmp(args[1], "--help") == 0) {
        fprintf(stdout, "%s", cd_help); 
        return;
    }

    change_dir(args[1]);
}
