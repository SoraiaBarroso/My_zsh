#include "my_zsh.h"

// Execute built-ins
void exec_builtin(Token* tokens, Environment* env)
{
    switch (tokens->command) {
        case CMD_CD:
            return cmd_cd(tokens);
        case CMD_PWD:
            return cmd_pwd();
        case CMD_ECHO:
            return cmd_echo(tokens, env);
        case CMD_EXIT:
            return cmd_exit();
        case CMD_SETENV:
            return cmd_setenv(tokens, env);
        case CMD_UNSETENV:
            return cmd_unsetenv(tokens, env);
        case CMD_WHICH:
            return cmd_which(tokens);
        case CMD_ENV:
            return cmd_env(env);
        default:
            return;
    }
}

int exec_binary(Token* tokens, Environment* env)
{
    char *path = find_binary(tokens->arr[0]);
    if (path == NULL) {
        printf("my_zsh: command not found: %s\n", tokens->arr[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    int status;
    pid = fork();

    if (pid == -1) {
        perror("fork");
        return -1;
    } else if (pid == 0) {
        // Child process
        if (execve(path, tokens->arr, env->env) == -1) {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int term_signal = WTERMSIG(status);
            if (term_signal == SIGSEGV) {
                write(STDERR_FILENO, "segmentation fault\n", 19);
            }
        }
    }
    return 0;
}
