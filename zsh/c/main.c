#include "../h/main.h"

#define MAX_BUFFER_SIZE 256
#define MAX_SUBSTRINGS 100

static char* prev_dir = NULL;

void _echo(char** substrings, char** env) {
    if (substrings[1][0] == '$') {
        char* tmp;
        if (!(tmp = getenv(&(substrings[1][1])))) {
            printf("echo: variable not found!\n");
            return;
        }
        printf("%s\n", tmp);
    }
    else _executor(substrings, env);
}

void save_path() {
    char* buffer;
    buffer = calloc(sizeof(char), MAX_BUFFER_SIZE);
    getcwd(buffer, MAX_BUFFER_SIZE);
    if (prev_dir != NULL)
    {
        free(prev_dir);
    }
    prev_dir = strdup(buffer);
    free(buffer);
}

void _cd(char** substrings) {
    char* buff;
    if (!substrings[1]) error_print("No such file or directory\n");
    else if (strcmp(substrings[1], "-") == 0) {
        if (prev_dir == NULL) {
            save_path();
            return;
        }
        buff = calloc(sizeof(char), MAX_BUFFER_SIZE);
        getcwd(buff, MAX_BUFFER_SIZE);
        chdir(prev_dir);
        free(prev_dir);
        prev_dir = strdup(buff);
        free(buff);
        return;
    }
    save_path();
    if (chdir(substrings[1]) != 0) {
        perror("chdir");
        return;
    }
}

void re_dir(char* substrings[], char** env) {
    if ((strcmp(substrings[0], "quit") == 0) ||(strcmp(substrings[0], "exit") == 0)) {
        deallocate(substrings);
        exit(EXIT_SUCCESS);
    }
    else if (strcmp(substrings[0], "env") == 0) _env(env);
    else if (strcmp(substrings[0], "setenv") == 0) {
        if (strchr(substrings[1], '=') != NULL) {
            short i = 1;
            char* token = strtok(substrings[1], "=");
            while (token != NULL) {
                substrings[i++] = strdup(token);
                token = strtok(NULL, "=");
            }
        }
        else if (strchr(substrings[2], '=') != NULL) {
            if (substrings[3] != NULL) {
                free(substrings[2]);
                substrings[2] = strdup(substrings[3]);
            }
        }
        if (setenv(substrings[1], substrings[2], 0) == -1) error_print("setenv: failure!\n");
    }
    else if (strcmp(substrings[0], "unsetenv") == 0) {
        if (unsetenv(substrings[1]) == -1) error_print("unsetenv:  failure!\n");
    }
    else if (strcmp(substrings[0], "echo") == 0) _echo(substrings, env);
    else if (strcmp(substrings[0], "cd") == 0) _cd(substrings);
    else _executor(substrings, env);   
}

void current_dir(char* buffer, size_t length, char* substrings[], char** env) {
    while(1) {
        clean_buf(buffer, length);
        getcwd(buffer, length);
        printf("[%s]>", buffer);
        clean_buf(buffer, length);
        if (getline(&buffer, &length, stdin) == -1) {
            if (prev_dir != NULL) free(prev_dir);
            break;
        }
        split(buffer, " ", substrings);
        if (!substrings[0]) continue;
        re_dir(substrings, env);
        deallocate(substrings);
    }
    free(buffer);
}

int main(int ac, char** av, char** env) {
    if (ac > 2 || !av) return 0;
    char* buffer = (char*)malloc(MAX_BUFFER_SIZE);
    size_t length = MAX_BUFFER_SIZE;
    char* substrings[MAX_SUBSTRINGS];
    current_dir(buffer, length, substrings, env);
    return 0;
}