#ifndef MY_ZSH_H
#define MY_ZSH_H
    #include <stdio.h> // getline perror
    #include <stdlib.h> // malloc free exit
    #include <sys/types.h> // opendir stat lstat, fstat,
    #include <sys/wait.h> // wait waitpid
    #include <sys/resource.h> //wait3, wait4
    #include <sys/time.h>
    #include <sys/stat.h>
    #include <dirent.h> // readdir closedir
    #include <unistd.h> // getcwd chdir fork close read write execve isatty getpid
    #include <fcntl.h> // open
    #include <signal.h> // kill, signal
    #include <string.h> //  strerror
    #include <errno.h> // perror

    #define MAX_SIZE    1024

    typedef enum {
        NONE,
        CMD_EXIT,
        CMD_CD,
        CMD_ECHO,
        CMD_PWD,
        CMD_WHICH,
        CMD_ENV,
        CMD_SETENV,
        CMD_UNSETENV,
    } Builtins_t;
    #define CMD_LAST    CMD_UNSETENV

    typedef struct Environment {
        char **env;
        int len;
    } Environment; 

    typedef struct Tokens {
        char* arr[MAX_SIZE];
        int len;
        Builtins_t command;
    } Token;

    /* The global variable to access the environment (null-terminated, format: key=value) */
    extern char **environ;

    /* Helper Functions */
    int     string_char(const char *str, char inc);
    int     str_len(const char *str);
    int     str_ncmp(const char *str1, const char *str2, size_t amt);
    char    *str_cpy(char *dst, const char *src);
    char    *str_cat(char *dst, const char *src);
    char    *find_binary(char *bin_path);

    /* Built-in implementations */
    void    cmd_exit();
    void    cmd_env(Environment* env);
    void    cmd_cd(Token* tokens);
    void    cmd_echo(Token* tokens, Environment* env);
    void    cmd_pwd();
    void    cmd_which(Token* tokens);
    void    cmd_setenv(Token* tokens, Environment* env);
    void    cmd_unsetenv(Token* tokens, Environment* env);

    /* Tokenization */
    int     tokens_validate(Token *tokens);
    Token*  tokens_parse(char* input);

    /* Execution */
    void    exec_builtin(Token* tokens, Environment* env);
    int     exec_binary(Token* tokens, Environment* env);

    /* Environment */
    Environment*    env_get();
    char*           env_build_var(Token* tokens);
    void            env_add_var(Environment* env, char* variable);
    char*           env_get_var(char* var_name);
#endif
