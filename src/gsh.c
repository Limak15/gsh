#include "gsh.h"

t_builtins builtins[] = {
    {.command = "exit", .func = &gsh_exit },
    {.command = "gsh_version", .func = &gsh_version },
    {.command = "history", .func = &history_cmd },
};

static int status = 0;

static void gsh_error_handler() {
    switch(errno) {
        case ENOENT:
            fprintf(stderr, "gsh: command not found\n");
            break;
        case EACCES:
            fprintf(stderr, "gsh: permission denied\n");
            break;
        case ENOEXEC:
            fprintf(stderr, "gsh: exec format error\n");
            break;
        default:
            perror("gsh");
    }

    exit(EXIT_FAILURE);
}

static char* gsh_prompt(void) {
    char* username = getlogin();
    char hostname[_SC_HOST_NAME_MAX + 1];
    char path[PATH_MAX];
    char* user_indicator = getuid() == 0 ? "#" : "$";

    if(username == NULL) {
        gsh_error_handler();
    }
    

    if(gethostname(hostname, sizeof(hostname)) == -1) {
        strcpy(hostname, "unknown-host");
    }


    if(getcwd(path, sizeof(path)) == NULL) {
        gsh_error_handler();
    }

    printf("\x1b[1m\x1b[38;5;39m%s@%s\x1b[0m:\x1b[1m\x1b[38;5;212m%s\x1b[0m%s ", username, hostname, path, user_indicator);
}

static char* gsh_readline(void) {
    char* buffer = NULL;
    size_t len;

    if(getline(&buffer, &len, stdin) == -1) {
        free(buffer);
        gsh_error_handler();
    }

    return buffer;
}

static void gsh_execute(char** argv, int argc) {

    // Check if typed command is a builtin function
    int builtins_size = sizeof(builtins) / sizeof(t_builtins); 
    
    for(int i = 0; i < builtins_size; ++i) {
        if(strcmp(builtins[i].command, argv[0]) == 0) {
            builtins[i].func(argv, argc);
            return;
        }
    }
    

    //If typed command is not builtin function launch it as seperate process
    pid_t pid = fork();

    if(pid < CHILD_PID) {
        gsh_error_handler();
    }

    if(pid == CHILD_PID) {
        if(execvp(argv[0], argv) == -1) {
            gsh_error_handler();
        }
    } else {
        wait(&status);
    }
}

static char** gsh_tokenize(char* line, int* token_count) {
    char** tokens;
    unsigned int pos = 0;
    size_t bufsize = BUFSIZ;

    tokens = malloc(bufsize * sizeof(*tokens));

    if(tokens == NULL) {
        gsh_error_handler();
    }

    for(char* token = strtok(line, COMMAND_DEL); token; token = strtok(NULL, COMMAND_DEL)) {
        tokens[pos++] = token;
        *token_count += 1;

        if(pos >= bufsize) {
            bufsize *= 2;

            tokens = realloc(tokens, bufsize * sizeof(*tokens));
        }
    }

    tokens[pos] = NULL; // Null-terminate the array of tokens

    return tokens;
}

void gsh_run(int argc, char** argv) {
    char* line = NULL;

    initialize_history();

    while(1) {
        // Print prompt
        gsh_prompt();

        // Read user input
        line = gsh_readline();

        history_add(line);

        // Tokenize input
        int tokc = 0;
        char** tokens = gsh_tokenize(line, &tokc);

        // Execute if builtin or fork/wait
        gsh_execute(tokens, tokc);        


        //Free tokens
        free(tokens);
    }
    
    free_history();
    free(line);
}
