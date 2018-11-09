

#ifndef CHARANALYSIS_H

#define CHARANALYSIS_H
//include************************************************************

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h> 

#include "Datastruct.h"
#include "selflib.h"
#include "history.h"
#include "press_tab.h"
#include "command_alias.h"

//******************************************************************

//define
 


// extern **************************************************
int shiftjudeg(int matchsource,char matchcode);
int in_table(int* table,int table_size,char code);
int charag_ctrlA(command_type* command);
int charag_ctrlB(command_type* command);
int charag_ctrlC(command_type* command);
int charag_ctrlD(command_type* command);
int charag_ctrlE(command_type* command);
int charag_ctrlF(command_type* command);
int charag_ctrlG(command_type* command);
int charag_ctrlH(command_type* command);
int charag_ctrlI(command_type* command);
int charag_ctrlJ(command_type* command);
int charag_ctrlK(command_type* command);
int charag_ctrlL(command_type* command);
int charag_ctrlM(command_type* command);
int charag_ctrlN(command_type* command);
int charag_ctrlO(command_type* command);
int charag_ctrlP(command_type* command);
int charag_ctrlQ(command_type* command);
int charag_ctrlR(command_type* command);
int charag_ctrlS(command_type* command);
int charag_ctrlT(command_type* command);
int charag_ctrlU(command_type* command);
int charag_ctrlV(command_type* command);
int charag_ctrlW(command_type* command);
int charag_ctrlX(command_type* command);
int charag_ctrlY(command_type* command);
int charag_ctrlZ(command_type* command);
int charag_ctrl27(command_type* command);
int charag_ctrl28(command_type* command);
int charag_ctrl29(command_type* command);
int charag_ctrl30(command_type* command);
int charag_ctrl31(command_type* command);
int charag_ctrl32(command_type* command);
int charag_altB(command_type* command);
int charag_altF(command_type* command);
int charag_altR(command_type* command);
int charag_altL(command_type* command);
int charag_altU(command_type* command);
int charag_altP(command_type* command);
int charag_alt46(command_type* command);
int charag_acE(command_type* command);
int charag_9165(command_type* command);
int charag_9166(command_type* command);
int charag_9168(command_type* command);
int charag_9167(command_type* command);
int charag_Del(command_type* command);
int charag_Ins(command_type* command);
int charag_Home(command_type* command);
int charag_Pgup(command_type* command);
int charag_Pgon(command_type* command);
int charag_End(command_type* command);



extern charag_functiontable_type charag_functiontable[51];
extern int screenlock;


//*******************************************************

#endif