
#ifndef COMMANDALIAS_H
#define COMMANDALIAS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h> 
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>

#include "Datastruct.h"
#include "selflib.h"
#include "misstake.h"
#include "process.h"

int inti_alias();
int get_aliasdir();
int open_and_creat_alias();
int write_essential_information();
int command_alias(int argc , char* arg[]);
int ass_alias_string(environment_variable* penv, char* string);
int develop_alias(char* command);
int gotonext_com(char* command, int* local); // return means end
int read_to_aliastable(int aliasfile_fd, envtable* alias_table);
int close_alias();
int write_back_aliasfile();
int isalias(char* string);

extern envtable alias_table;
extern char aliastable_dir[DIRECTORY_SIZE];
extern char* aliastable_name;
extern int aliasfile_fd;
extern int ifchange_table;  // 1 : wo need rewrite
extern char aliasfile_head[];

#endif