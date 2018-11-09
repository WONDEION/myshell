
#ifndef COMMANDEXIT_H
#define COMMANDEXIT_H

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>  
#include <malloc.h>
#include <string.h>

#include "initialize.h"

int command_exit(int argc, char* arg[]);

#endif