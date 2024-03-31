#include "../h/support_func.h"

void clean_buf(char* buffer, size_t length) {
    memset(buffer, 0, length);
}

void deallocate(char* substrings[]) {
    for (int i = 0; substrings[i] != NULL; i++) free(substrings[i]);
}

void error_print(char* message) {
    printf("%s", message);
}

void _env(char** env) {
    for (int i = 0; env[i] != NULL; i++) printf("%s\n", env[i]); 
}

void convert(char* substrings[]) {
    short i = 1;
    char* token = strtok(substrings[1], "=");
    while (token != NULL) {
        substrings[i++] = strdup(token);
        token = strtok(substrings[1], "=");
    }
}

char* real_copy(char* token, char* command) {
    short length = (strlen(token) + strlen(command) + 2);
    char* tmp = calloc(length, sizeof(char));
    strcat(tmp, token);
    strcat(tmp, "/");
    strcat(tmp, command);
    return tmp;
}

short _accessible(char* command, char** answer) {
    char* path = malloc(sizeof(char) * PATH_MAX);
    strcpy(path, getenv("PATH"));
    char* token = strtok(path, ":");
    char* tmp;
    while (token != NULL) {
        tmp = real_copy(token, command);
        if (access(tmp, X_OK) == 0) {
            *answer = strdup(tmp);
            free(path);
            free(tmp);
            return 1;
        }
        token = strtok(NULL, ":");
        free(tmp);
    }
    free(path);
    return 0;
}

short _executor(char** substrings, char** env) {
    pid_t pid;
    int status;
    char* path = NULL;
    if (strstr(substrings[0], "./") != NULL) {
        path = realpath(substrings[0], path);
        if (access(path, X_OK) != 0) {
            error_print("command not found\n");
            return 0;
        }
        if (path == NULL) {
            error_print("No such file or directory\n");
            return 0;
        }
    }
    else if (_accessible(substrings[0], &path) == 0) {
        free(path);
        error_print("command not found\n");
        return 0;
    }
    pid = fork();
    if (pid == 0) {
        if (execve(path, substrings, env) == -1) perror("lsh");
        exit(EXIT_FAILURE);
    }
    else {
        do{
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        short signal = WTERMSIG(status);
        if (signal == SIGSEGV) error_print("segmentation fault\n");
    }
    free(path);
    return 1;
}