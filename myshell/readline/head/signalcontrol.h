
#ifndef SIGNALCONTROL_H
#define SIGNALCONTROL_H


#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h> 

#include "Datastruct.h"
#include "selflib.h"
#include "process.h"
#include "readline.h"

int signal_Adaptation();
void signal_sigwinch(int sig);
void signal_sigint(int sig);  // ^C
void signal_sigtstp(int sig);  // ^Z   ^Y
void signal_sigquit(int sig);  // "^\"
void signal_sigchld(int sig);
void signal_sigusr1(int sig);

extern int sonwait;
extern int fatherwait;
extern int wait_spid;

#endif