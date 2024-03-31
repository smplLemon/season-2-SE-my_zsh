#ifndef MY_ZSH_H_SUPPORT_FUNC_H
#define MY_ZSH_H_SUPPORT_FUNC_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <linux/limits.h>

void _env(char** env);
void error_print(char* message);
void convert(char* substrings[]);
void deallocate(char* substrings[]);
void clean_buf(char* buffer, size_t length);
short _executor(char** substrings, char** env);
short _accessible(char* command, char** answer);
char* real_copy(char* token, char* command);

#endif