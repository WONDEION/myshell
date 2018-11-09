
#ifndef COMMANDSOURCE_H
#define COMMANDSOURCE_H

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h> 
#include <malloc.h>
#include <string.h>
#include <sys/stat.h>  // stat
#include <fcntl.h>   // open

#include "Datastruct.h"
#include "selflib.h"
#include "readline.h"
#include "command_cd.h"
#include "main.h"

int command_source(int argc, char* arg[]);
int test_file(char* filename);

#endif