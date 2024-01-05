#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include </mnt/d/Code/my_zsh/h/my_string.h>

#define MAX_BUFFER_SIZE 256
char *PWD = NULL;

void printEnv(char **env)
{
    for (int i = 0; env[i]; i++)
    {
        printf("%s\n", env[i]);
    }
}

void errorMessage(char *str)
{
    write(2, str, my_strlen(str));
}

int startBashCommand(char *command, char **path)
{
    str_array *arr = split(getenv("PATH"), ':');
    *path = calloc(sizeof(char), (MAX_BUFFER_SIZE + 1));
    for (int i = 0; i < arr->count; i++)
    {
        my_strcpy(*path, arr->array[i]);
        *path = my_strcpy_pro(*path, command, '/');
        // printf("path = %s\n", *path);
        if (access(*path, X_OK) == 0)
        {
            deallocate_str_array(arr);
            return 1;
        }
        fill_null(*path, MAX_BUFFER_SIZE);
    }
    deallocate_str_array(arr);
    return 0;
}

int executor(char **args, char **env)
{
    // printf("(%s)", args[0]);
    pid_t pid;
    int status;
    char *path = NULL;
    if (my_strncmp(args[0], "./", 2) == 0)
    {
        path = realpath(args[0], NULL);
        if (path == NULL)
        {
            errorMessage("No such file or directory\n");
            return 0;
        }
        else if (access(path, X_OK) != 0)
        {
            errorMessage("command not found\n");
            return 0;
        }
    }
    else if (startBashCommand(args[0], &path) == 0)
    {
        free(path);
        errorMessage("command not found\n");
        return 0;
    }
    // printf("%s\n", path);
    // after this function, execution has now two processes: a parent and a child.
    // how to determine which one is which?
    pid = fork();
    if (pid == 0)
    {
        // Child process
        if (execve(path, args, env) == -1)
        { // Executing the command.
            perror("lsh");
        }
        free(path);
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        // Parent process
        do
        {
            // wpid = waitpid(pid, &status, WUNTRACED);
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        if (WTERMSIG(status) == SIGSEGV)
        {
            errorMessage("segmentation fault\n");
            return 0;
        }
        // Wait for process child to be done.
    }
    free(path);
    return 1;
}

void printForEnv(str_array *arr, char **env)
{
    if (arr->array[1][0] == '$')
    {
        char *temp = NULL;
        if (!(temp = getenv(&arr->array[1][1])))
        {
            errorMessage("echo: varibal not found\n");
            return;
        }
        printf("%s\n", temp);
    }
    else
    {
        executor(arr->array, env);
    }
}

void functionForCd(str_array *arr)
{
    if (!arr->array[1])
    {
        errorMessage("No such file or directoriy\n");
    }
    else if (arr->array[1][0] == '-')
    {
        if (PWD == NULL)
        {
            PWD = calloc(sizeof(char), MAX_BUFFER_SIZE + 1);
            getcwd(PWD, MAX_BUFFER_SIZE);
            return;
        }
        char *temp = my_strdup(PWD);
        free(PWD);
        PWD = calloc(sizeof(char), MAX_BUFFER_SIZE + 1);
        getcwd(PWD, MAX_BUFFER_SIZE);
        chdir(temp);
        free(temp);
        return;
    }

    if (PWD != NULL)
    {
        free(PWD);
    }
    PWD = calloc(sizeof(char), MAX_BUFFER_SIZE + 1);
    getcwd(PWD, MAX_BUFFER_SIZE);
    if (chdir(arr->array[1]) != 0)
    {
        perror("chdir");
        return;
    }
}

void commandChecker(str_array *arr, char **env)
{
    // printf("arr[0] = %s\n", arr->array[0]);
    if (my_strcmp(arr->array[0], "quit") == 0 || my_strcmp(arr->array[0], "exit") == 0)
    {
        // printf("asd\n");
        deallocate_str_array(arr);
        exit(EXIT_SUCCESS);
    }
    else if (my_strcmp(arr->array[0], "env") == 0)
    {
        printEnv(env);
    }
    else if (my_strcmp(arr->array[0], "setenv") == 0)
    {
        if (setenv(arr->array[1], arr->array[2], 0) == -1)
        {
            errorMessage("setenv: killed\n");
        }
    }
    else if (my_strcmp(arr->array[0], "unsetenv") == 0)
    {
        if (unsetenv(arr->array[1]) == -1)
        {
            errorMessage("unsetenv: killed\n");
        }
    }
    else if (my_strcmp(arr->array[0], "echo") == 0)
    {
        printForEnv(arr, env);
    }
    else if (my_strcmp(arr->array[0], "cd") == 0)
    {
        functionForCd(arr);
    }
    else
    {
        executor(arr->array, env);
        return;
    }
}

void promp(char **env)
{
    char *strForPath = calloc(sizeof(char), (MAX_BUFFER_SIZE + 1));
    // char *strForRead = calloc;
    str_array *arr = NULL;
    size_t len = MAX_BUFFER_SIZE;
    size_t str_length = 0;
    while (1)
    {
        getcwd(strForPath, MAX_BUFFER_SIZE);
        printf("[%s]>", strForPath);
        fill_null(strForPath, MAX_BUFFER_SIZE);
        fflush(stdout);
        if (getline(&strForPath, &len, stdin) == -1)
        {
            break;
        }
        str_length = my_strlen(strForPath);
        if (str_length > 0 && strForPath[str_length - 1] == '\n')
        {
            strForPath[str_length - 1] = '\0';
        }
        arr = split(strForPath, ' ');
        commandChecker(arr, env);
        fill_null(strForPath, MAX_BUFFER_SIZE);
        deallocate_str_array(arr);
    }
    free(strForPath);
}

int main(int argc, char **argv, char **env)
{
    if (argc || argv)
    {
    }
    promp(env);
    return 0;
}