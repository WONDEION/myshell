
#ifndef COMMNADUNALIAS_H
#define COMMNADUNALIAS_H

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
#include "command_alias.h"

int command_unalias(int argc , char* arg[]);
int delete_allalias();


#endif