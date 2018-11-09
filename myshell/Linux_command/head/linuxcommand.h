
#ifndef LINUXCOMMAND_H
#define LINUXCOMMAND_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <dirent.h>

#include "Datastruct.h"
#include "selflib.h"
#include "process.h"
#include "signalcontrol.h"
#include "termiocontrol.h"
#include "execfile.h"


int get_path();
int malloc_command_path(int n);
int free_command_path();
int test_linuxcommand(char* name, char* directory);
int get_linuxcommand_dir(char* name, char* directory);
int execute_linuxcommand(char* command);
int islinuxcommand(char* command);


extern char** command_path;
extern int command_path_size;

#endif