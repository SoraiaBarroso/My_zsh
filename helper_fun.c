#include "my_zsh.h"

/* Returns the position of the character in the string (if the string is neither null nor empty) */
int string_char(const char *str, char inc) 
{
    if (str == NULL || str[0] == '\0') return -1;
    for (size_t i = 0; str[i] != '\0'; i++) { if (str[i] == inc) return i; }
    return -1;
}

/* Returns the string length (or 0 if the string is null or empty) */
int str_len(const char *str) 
{
    if (str == NULL || str[0] == '\0') return 0;
    size_t i = 0;
    while (str[i] != '\0') i++;
    return i;
}

int str_ncmp(const char *str1, const char *str2, size_t amt) 
{
    if (str1 == NULL && str2 == NULL) return 0;
    if (str1 == NULL) return -1;
    if (str2 == NULL) return 1;
    for (size_t i = 0; i < amt; i++) {
        if (str1[i] < str2[i]) return -1;
        if (str1[i] > str2[i]) return 1;
    }
    return 0;
}

char *str_cpy(char *dst, const char *src) 
{
    char *ptr = dst;
    for (size_t i = 0; src[i] != '\0'; i++) *dst++ = src[i];
    *dst = '\0';
    return ptr;
}

char *str_cat(char *dst, const char *src) 
{
    char *ptr = dst;
    while (*dst != '\0') dst++;
    for (size_t i = 0; src[i] != '\0'; i++) *dst++ = src[i];
    *dst = '\0';
    return ptr;
}

char* env_get_var(char* var_name)
{
    size_t name_len = strlen(var_name);
    for (int i = 0; environ[i] != NULL; i++) {
        if (strncmp(environ[i], var_name, name_len) == 0) {
            return strdup(environ[i] + name_len);
        }
    }
    return NULL;
}

int envLen() 
{
    int len = 0;

    for (len = 0; environ[len] != NULL; len++);

    return len;
}

// copy global environ
Environment* env_get()
{
    Environment* env = malloc(sizeof(Environment));
    env->len = envLen();
    env->env = malloc(sizeof(char*) * (env->len + 2));

    for (int i = 0; environ[i] != NULL; i++) {
        env->env[i] = malloc(strlen(environ[i]) + 2);
        strcpy(env->env[i], environ[i]);
        strcat(env->env[i], "\n");
    }

    env->env[env->len] = NULL; 
    return env;
}

char* env_build_var(Token* tokens)
{
    size_t len = strlen(tokens->arr[1]) + 1 + strlen(tokens->arr[2]) + 1;
    char *new_var = malloc(len);

    strcpy(new_var, tokens->arr[1]);
    strcat(new_var, "=");
    strcat(new_var, tokens->arr[2]);
    
    return new_var;
}

void env_add_var(Environment* env, char* variable)
{
    env->len += 1;
    env->env = realloc(env->env, sizeof(char*) * (env->len + 1));

    char *new_var = malloc(strlen(variable) + 2);
    strcpy(new_var, variable);
    strcat(new_var, "\n");

    env->env[env->len - 1] = new_var;
    env->env[env->len] = NULL;
}

char *find_binary(char *bin_path) 
{
    if (bin_path == NULL) return NULL;

    char *path = env_get_var("PATH=");
    if (path == NULL) return NULL;

    char *dir = strtok(path, ":");
    char *full_path = malloc(sizeof(char) * MAX_SIZE);
    struct stat f_stat;

    while(dir != NULL) {
        strcpy(full_path, dir);
        strcat(full_path, "/");
        strcat(full_path, bin_path);

        if (stat(full_path, &f_stat) == 0 && S_ISREG(f_stat.st_mode)) {
            return full_path;
        }

        dir = strtok(NULL, ":");
    }

    if (bin_path[0] == '.' && stat(bin_path, &f_stat) == 0 && S_ISREG(f_stat.st_mode)) {
        strcpy(full_path, bin_path);
        return full_path;
    }

    free(full_path);
    return NULL;
}