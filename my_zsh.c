#include "my_zsh.h"

void prompt()
{
    char cwd[MAX_SIZE];
    getcwd(cwd, sizeof(cwd));

    write(STDIN_FILENO, "[", 1);
    write(STDIN_FILENO, cwd, strlen(cwd));
    write(STDIN_FILENO, "]> ", 3);
}

int main(int argc, char* argv[])
{
    if (!argv[0] || argc > 1) {
        perror("Error running program");
    }

    size_t capacity = 0;
    char *input = NULL;
    long ch;

    Environment *env = env_get();

    while (1) {
        prompt();
        ch = getline(&input, &capacity, stdin);

        if (input[0] == '\n') {
            continue;
        } else if (ch <= 0) {
            exit(0);
        }

        Token *tokens = tokens_parse(input);
        if (tokens_validate(tokens) == 0) {
            if (tokens->command == NONE) {
                exec_binary(tokens, env);
            } else {
                exec_builtin(tokens, env);
            }
        } else {
            for (int i = 0; i < tokens->len; i++) {
                free(tokens->arr[i]);
            }
            free(tokens);
            continue;
        }
    }
}