
#ifndef COMMANDCD_H
#define COMMANDCD_H

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <pwd.h>

#include "Datastruct.h"
#include "selflib.h"
#include "misstake.h"
#include "process.h"


int command_cd(int argc,char* arg[]);
int cd_option(int argc,char* arg[],char ch,char* dir);
int get_chdir(char* arg[],char* dir);
int set_nowdir(char* dir);
int develop_abbdir(char* dir);
int testdir(char* dir);
int isdirchar(char ch);
int islegaldirectory(char* dir);

#endif