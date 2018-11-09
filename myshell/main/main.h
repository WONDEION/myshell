
#ifndef MAIN_H
#define	MAIN_H 

// include***************************************************
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <termio.h>


#include "readline.h"
#include "Datastruct.h"
#include "selflib.h"
#include "initialize.h"
#include "history.h"
#include "analysis.h"

//***************************************************


//extren***************************************************

int printcommand(command_type* pcommand);
int runcommand(command_type* pcommand);   //
int printhello();

//***************************************************


#endif