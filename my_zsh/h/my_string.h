#ifndef ZSH_H_MY_STRING_H_
#define ZSH_H_MY_STRING_H_

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "./structures.h"

int my_strlen(char *);
int my_strcmp(char *, char *);
int my_strncmp(char *p1, char *p2, int size);
char *my_strcpy(char *str1, char *str2);
char *my_strcpy_pro(char *str1, char *str2, char add);
int find_pieces(char *, char);
int pieces_len(int, char *, char);
char *my_scanf();
char *my_strdup(char *);
void f_delete_space(char *);
void fill_null(char *, int);
str_array *split(char *, char);
void deallocate_str_array(str_array *s_array);

#endif