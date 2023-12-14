#include "my_zsh.h"

void cmd_env(Environment* env)
{
    for (int i = 0; i < env->len; i++) {
        printf("%s", env->env[i]);
    }
}

char* cd_prev_dir;

void cmd_cd(Token* tokens)
{
    char current_dir[MAX_SIZE];

    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("getcwd");
        return;
    }
   
    // If no args or cd ~ go to home directory
    if (tokens->len == 2 || tokens->arr[1][0] == '~') {
        char *home_dir = env_get_var("HOME=");
        chdir(home_dir);
        free(home_dir);
    } else if (tokens->arr[1][0] == '-') { 
        if (cd_prev_dir == NULL) {
            printf("cd: no previous directory\n");
        } else {
            chdir(cd_prev_dir);
        }
    } else {
        if (chdir(tokens->arr[1]) != 0) {
            perror("chdir");
        }
    }

    // Update dir
    free(cd_prev_dir);
    cd_prev_dir = strdup(current_dir);
}

void cmd_which(Token* tokens)
{
    char *path = NULL;

    for (int i = 0; environ[i] != NULL; i++) {
        if (strncmp(environ[i], "PATH=", 5) == 0) {
            path = strdup(environ[i]);
            break;
        }
    }

    char* dir = strtok(path, ":");
    int found = 0;
    struct stat fileStat;

    char *full_path = malloc(sizeof(char) * MAX_SIZE);

    while(dir != NULL)
    {
        strcpy(full_path, dir);
        strcat(full_path, "/");
        strcat(full_path, tokens->arr[1]);

        if (stat(full_path, &fileStat) == 0) {
            if (S_ISREG(fileStat.st_mode)) {
                printf("%s\n", full_path);
                found = 1;
                break;
            }
        }
        dir = strtok(NULL, ":");
    }

    if (!found) {
        printf("My_zsh: command not found: %s\n", tokens->arr[1]);
    }

    free(path);
    free(full_path);
}

void cmd_exit()
{
    exit(0);
}

void cmd_echo(Token* tokens, Environment* env)
{
    // Print environment variables
    if (tokens->arr[1][0] == '$')
    {
        char* var = strtok(tokens->arr[1], "$");

        for (int i = 0; i < env->len; i++) {
            if (strncmp(env->env[i], var, strlen(var)) == 0) {
                printf("%s", env->env[i] + strlen(var) + 1);
                return;
            }
        }
    }
    else {
        for (int i = 1; i < tokens->len - 1; i++) {
            printf("%s ", tokens->arr[i]);
        }
        printf("\n");
    }
}

void cmd_pwd()
{
    char *dir = NULL;
    dir = (char*)malloc(MAX_SIZE * sizeof(char));

    if (getcwd(dir, MAX_SIZE) != NULL) {
        printf("%s\n", dir);
    } else {
        free(dir);
        perror("getcwd");
    }

    free(dir);
}

void cmd_setenv(Token* tokens, Environment* env)
{
    // if no arguments, print env
    if (tokens->arr[1] == NULL) {
        cmd_env(env);
        return;
    }

    int eq_in_key = string_char(tokens->arr[1], '=') != -1;
    if (tokens->arr[2] == NULL && !eq_in_key) {
        printf("Error: variable name incorrect\n");
        return;
    }

    char *new_var = eq_in_key ? tokens->arr[1] : env_build_var(tokens);
    env_add_var(env, new_var);
}

void cmd_unsetenv(Token* tokens, Environment* env)
{
    for (int i = 0; i < env->len; i++) {
        char* var = tokens->arr[1];
        if (strncmp(env->env[i], var, strlen(var)) == 0) {
            free(env->env[i]);
            env->len--;
            break;
        }
    }
}
