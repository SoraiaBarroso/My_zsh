#include "my_zsh.h"

int tokens_validate(Token *tokens)
{
    switch (tokens->command) {
        case CMD_EXIT:
        case CMD_PWD:
        case CMD_ENV:
            // no args besides command & terminator
            if (tokens->len > 2) {
                printf("%s: too many arguments\n", tokens->arr[0]);
                return 1;
            }
            break;
        case CMD_SETENV:
            // 1-2 args (+ command & terminator)
            if (tokens->len < 3) {
                printf("%s: more arguments needed\n", tokens->arr[0]);
                return 1;
            } else if (tokens->len > 4) {
                printf("%s: too many arguments\n", tokens->arr[0]);
                return 1;
            }
            break;
        case CMD_UNSETENV:
            // 1 arg exactly (+ command & terminator)
            if (tokens->len < 3) {
                printf("%s: more arguments needed\n", tokens->arr[0]);
                return 1;
            } else if (tokens->len > 3) {
                printf("%s: too many arguments\n", tokens->arr[0]);
                return 1;
            }
            break;
        case CMD_WHICH:
            // at least 1 arg (+ command & terminator)
            if (tokens->len <= 2) {
                printf("%s: more arguments needed\n", tokens->arr[0]);
                return 1;
            }
            break;
        default:
            return 0;
    }
    return 0;
}

void tokens_classify(Token* tokens)
{
    tokens->command = NONE;

    char *cmd_list[CMD_LAST + 2] = {
            [CMD_CD] = "cd",
            [CMD_ECHO] = "echo",
            [CMD_EXIT] = "exit",
            [CMD_SETENV] = "setenv",
            [CMD_UNSETENV] = "unsetenv",
            [CMD_PWD] = "pwd",
            [CMD_WHICH] = "which",
            [CMD_ENV] = "env",
            [CMD_LAST + 1] = NULL,
    };

    char *key;
    for (int id = 1; (key = cmd_list[id]) != NULL; id++) {
        if (strcmp(tokens->arr[0], key) == 0) {
            tokens->command = id;
            return;
        }
    }
}

Token* tokens_parse(char* input)
{
    Token* tokens = malloc(sizeof(Token));
    tokens->len = 0;

    int i = 0, index = 0;

    while (input[index] != '\n') {    
        while (input[index] == ' ') index++;

        if (input[index] != '\n') {
            int start = index;

            while (input[index] != ' ' && input[index] != '\n') index++;
           
            int len = index - start;
            tokens->arr[i] = malloc(len + 1);
            strncpy(tokens->arr[i], input + start, len);
            tokens->arr[i][len] = '\0';

            tokens->len++;
            i++;
        }   
    }

    // NULL terminated {ls, -l, NULL}
    tokens->arr[i] = NULL;
    tokens->len++;

    tokens_classify(tokens);
    return tokens;
}