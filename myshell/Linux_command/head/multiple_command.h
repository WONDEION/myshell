
#ifndef MULTIPLECOMMAND_H
#define MULTIPLECOMMAND_H

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
#include "process.h"
#include "main.h"
#include "execfile.h"
#include "analysis.h"

int iscommandgroup(char* command);
int gotocommandgroup(char* command);
int build_commandgroup(char* command);
int nobuild_commandgroup(char* command);
int commandgroup(char* command);
int gotoqueuecommand(char* command);
int isqueuecommand(char* command);
int execqueuecommand(char* command);  // 0 means normal  1 means unnormal
int is_orwithspacer(char* pchar);

#endif