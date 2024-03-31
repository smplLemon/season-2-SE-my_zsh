#include "../h/supportive_functions.h"

void buf_clear(char* buffer, size_t length) {
    memset(buffer, 0, length);
}

void de_loc_substring(char* substrings[]) {
    for (short i = 0; substrings[i] != NULL; i++) free(substrings[i]);
}

void error(char* message) {
    write(2, message, strlen(message));
}

void printing_env(char** env) {
    for (short i = 0; env[i] != NULL; i++) printf("%s\n", env[i]);
}

void convert_form(char* substrings[]) {
    short i = 1;
    char* token = strtok(substrings[1], "=");
    while (token != NULL) {
        substrings[i++] = strdup(token);
        token = strtok(substrings[1], "=");
    }
}

char* my_copy(char* token, char* command) {
    short length = (strlen(token) + strlen(command) + 2);
    char* tmp = calloc(length, sizeof(char));
    strcat(tmp, token);
    strcat(tmp, "/");
    strcat(tmp, command);
    return tmp;
}

short is_access(char* command, char** answer) {
    char* path = malloc(sizeof(char) * PATH_MAX);
    strcpy(path, getenv("PATH"));
    char* token = strtok(path, ":");
    char* tmp;
    while (token != NULL) {
        tmp = my_copy(token, command);
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

short is_executor(char** substrings, char** env) {
    pid_t pid;
    int status;
    char* path = NULL;
    if (strstr(substrings[0], "./") != NULL)
    {
        path = realpath(substrings[0], path);
        if (access(path, X_OK) != 0) {
            error("command not found\n");
            return 0;
        }
        if (path == NULL) {
            error("No such file or directory\n");
            return 0;
        }
    }
    else if (is_access(substrings[0], &path) == 0) {
        free(path);
        error("command not found\n");
        return 0;
    }
    pid = fork();
    if (pid == 0) {
        if (execve(path, substrings, env) == -1){
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    }
    else {
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        int signal = WTERMSIG(status);
        if (signal == SIGSEGV) {
            error("segmentation fault\n");
        }
    }
    free(path);
    return 1;
}