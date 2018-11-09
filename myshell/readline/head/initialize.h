
#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <termio.h>
#include <sys/ioctl.h>
#include <signal.h>  
#include <pwd.h>
#include <sys/stat.h>
#include <malloc.h>

#include "readline.h"
#include "Datastruct.h"
#include "history.h"
#include "signalcontrol.h"
#include "process.h"
#include "linuxcommand.h"
#include "termiocontrol.h"
#include "command_alias.h"
#include "analysis.h"

int initialize();
int bulid_session();
int quitclear();
int getcdir();
int datastuct_init();
int getwin_info();
int get_homedir();
int set_workdir();
int getuserinfo();
int inti_history();

#endif