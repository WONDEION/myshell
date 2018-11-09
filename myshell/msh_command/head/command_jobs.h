 #ifndef COMMANDJOBS_H
 #define COMMANDJOBS_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>  
#include <malloc.h>
#include <sys/ioctl.h>
#include <string.h>

#include "Datastruct.h"
#include "selflib.h"
#include "misstake.h"
#include "process.h"
#include "termiocontrol.h"
#include "signalcontrol.h"

int command_jobs(int argc, char* arg[]);
int command_fg(int argc, char* arg[]);
int command_bg(int argc, char* arg[]);
int command_kill(int argc, char* arg[]);

#endif