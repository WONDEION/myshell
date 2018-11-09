
#ifndef SHELLCOMMAND_H
#define SHELLCOMMAND_H

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

#include "Datastruct.h"
#include "selflib.h"

#include "history.h"
#include "command_cd.h"
#include "command_source.h"
#include "command_alias.h"
#include "command_unalias.h"
#include "command_jobs.h"
#include "command_type.h"
#include "command_exit.h"
#include "command_let.h"

int devecalculation(char* command);
int gotobracketscommand(char* command);
int isshellcommand(char* command);
int gotoshellcommand(char* command);
int isbarcecommand(char* command);
int exec_computecommand(char* command, int* pvalue);

extern char* shellcommand_nametable[];
extern int (*(shellcommand_functiontable[]))(int,char**);

#endif