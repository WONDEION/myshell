
#ifndef COMMANDEXECFILE_H
#define COMMANDEXECFILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <malloc.h>
#include <errno.h>

#include "Datastruct.h"
#include "selflib.h"
#include "misstake.h"
#include "command_cd.h"
#include "linuxcommand.h"
#include "termiocontrol.h"
#include "signalcontrol.h"

int isexecfile(char* command);
int testfile(char* filename);
int execfile(char* command);
int fork_exec(char* dir,char* filename, int argc, char** arg);
int adjustfilename(char* filename);

#endif

