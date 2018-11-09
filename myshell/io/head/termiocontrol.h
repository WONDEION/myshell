
#ifndef TERMIOCONTROL_H
#define TERMIOCONTROL_H 

#include <stdio.h>
#include <termio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int intimode();
int setmode();
int back_mode();
int set_run_termode();

#endif