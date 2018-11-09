
#ifndef COMMANDTYPE_H
#define COMMANDTYPE_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>

#include "Datastruct.h"
#include "selflib.h"
#include "misstake.h"
#include "shellcommand.h"
#include "command_alias.h"

int shellcommand_type(int argc, char* arg[]);
int type_display();

#endif