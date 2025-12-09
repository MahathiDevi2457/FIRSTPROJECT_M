#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // fork, chdir, execvp
#include <sys/wait.h>   // waitpid
#include <errno.h>      // errno
#include <limits.h>     // PATH_MAX

#define MAX_INPUT_SIZE 1024
#define MAX_TOKENS 64
#define DELIM " \t\r\n\a"   // delimiters for tokenizing

// Function declarations
void shell_loop(void);
char *read_line(void);
char **split_line(char *line);
int execute_command(char **args);
int launch_process(char **args);

// Built-in commands
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);

// List of built-in commands
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &shell_cd,
    &shell_help,
    &shell_exit
};

int num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int main(void) {
    // Main loop of the shell
    shell_loop();
    return 0;
}

void shell_loop(void) {
    char *line;
    char **args;
    int status = 1;

    do {
        // Print prompt with current directory
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s$ ", cwd);
        } else {
            perror("getcwd");
            printf("myshell$ ");
        }

        line = read_line();
        if (line == NULL) {
            // EOF (Ctrl + D)
            printf("\n");
            break;
        }

        args = split_line(line);
        if (args[0] != NULL) {
            status = execute_command(args);
        }

        // Free allocated memory
        free(line);
        free(args);

    } while (status);
}

// Read a line from stdin
char *read_line(void) {
    char *buffer = NULL;
    size_t bufsize = 0; // getline will allocate
    ssize_t nread;

    nread = getline(&buffer, &bufsize, stdin);
    if (nread == -1) {
        // Error or EOF
        if (feof(stdin)) {
            // End of file (Ctrl + D)
            free(buffer);
            return NULL;
        } else {
            perror("getline");
            free(buffer);
            return NULL;
        }
    }

    return buffer;
}

// Split a line into tokens (arguments)
char **split_line(char *line) {
    int bufsize = MAX_TOKENS;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, "myshell: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += MAX_TOKENS;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "myshell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

// Execute shell built-in or launch program
int execute_command(char **args) {
    if (args[0] == NULL) {
        // Empty command
        return 1;
    }

    // Check if the command is a built-in
    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    // Not a built-in, launch external program
    return launch_process(args);
}

// Launch a program using fork and execvp
int launch_process(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("myshell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("myshell");
    } else {
        // Parent process waits for child
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

// Built-in: cd
int shell_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "myshell: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("myshell");
        }
    }
    return 1;
}

// Built-in: help
int shell_help(char **args) {
    (void) args; // unused
    printf("Mini Shell - simple command interpreter in C\n");
    printf("Built-in commands:\n");

    for (int i = 0; i < num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

// Built-in: exit
int shell_exit(char **args) {
    (void) args; // unused
    return 0; // return 0 -> stops shell_loop
}
