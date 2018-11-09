
#ifndef ANALISIS_H
#define ANALISIS_H


#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

#include "Datastruct.h"
#include "selflib.h"
#include "process.h"
#include "history.h"
#include "redirect.h"
#include "linuxcommand.h"
#include "shellcommand.h"
#include "execfile.h"
#include "command_alias.h"
#include "multiple_command.h"

int intianalysis();
int close_all_switch();
int open_all_switch();
int analysis_command_primary(char* command);
int command_develop(char* command);
int develop_directory(char* command);
int develop_history(char* command);
int remove_quote(char* command);
int get_escape_character(char* string, char* pch);
int analysis_singlecommand(char* command);
int clear_superfluous_space(char* string);
int tab_to_space(char* string);
int process_input(char* command);

extern int develop_his_switch;
extern int develop_env_switch;
extern int develop_cal_switch;
extern int develop_ali_switch;
extern int develop_que_switch;

#endif