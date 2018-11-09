
#ifndef REDIRECT_H
#define REDIRECT_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

#include "Datastruct.h"
#include "selflib.h"
#include "command_cd.h"
#include "main.h"
#include "analysis.h"
#include "termiocontrol.h"

#define PIPEREAD 0
#define PIPEWRITE 1

int isredirect(char* command);   //-1 error 0 no redirect 1 have redirect
int gotoredirect(char* command);
int execredirect(char* command);
int all_stop(int* state_array,int num);
int redi_file(int file_fd, char* command,int mode);
int get_command_number(char* command);
int get_pipe_number(char* command);
int get_file_fd(char* command,int* pmode);
int get_next_command(char* command ,char* nextcom,int* pcount);
int** malloc_pipearrry(int num);
void free_pipearray(int** pipe_array,int num);

#endif