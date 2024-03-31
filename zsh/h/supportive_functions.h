#ifndef MY_ZSH_H_SUPPORTIVE_FUNCTIONS_H_
#define MY_ZSH_H_SUPPORTIVE_FUNCTIONS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <linux/limits.h>

void printing_env(char** env);
void error(char* message);
void convert_form(char* substrings[]);
void de_loc_substring(char* substrings[]);
void buf_clear(char* buffer, size_t length);
short is_executor(char** substrings, char** env);
short is_access(char* command, char** answer);
char* my_copy(char* token, char* command);

#endif