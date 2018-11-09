
#ifndef READLINE_H
#define READLINE_H



//include********************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <sys/ioctl.h>

#include "charanalysis.h"
#include "Datastruct.h"
#include "misstake.h"
#include "selflib.h"

//***************************************************************************




//extern*********************************************************************


int readline(int fd, command_type* pcommand);
int getline_m(int fd, command_type* pcommand);


char msh_getchar(int fd);
int isfinished(command_type* pcommand);
int ischangeline(command_type* pcommand);
int printcommandinfo(command_type* pcommand);
int add_and_print(command_type* pcommand,char inputchar);
int print_nowscreen();


//***************************************************************************


#endif