
#ifndef PRESSTAB_H
#define PRESSTAB_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

#include "Datastruct.h"
#include "selflib.h"
#include "shellcommand.h"
#include "readline.h"

int handle_tab(command_type* pcommand);
int get_word(command_type* pcommand,char* word); // -1 no word  
int word_type(char* word,int i);
int find_command_and_develop(char* word);
int find_file_and_develop(char* word,int mode); //Put in word
int get_match_word_local(char const* word);
int match_in_dir(char* dir,char* word,int mode); // Put in word
int tab_match_string(char const* source,char const* object);
int print_allcommand(command_type* pcommand,char* word);
int print_allfile(command_type* pcommand,char* word);

#endif