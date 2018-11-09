
#ifndef HISTORY_H
#define HISTORY_H
//#includ**********************************************************
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <malloc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#include "Datastruct.h"
#include "selflib.h"
//#include "process.h"

#define HISTABLE_SIZE 1024
#define FILEHEAD_SIZE 512
#define WRAP_NUMBER 2
#define CACHING_LENGTH 512
#define FILE_WRAP_STRING "\r\n"
#define NUMBERSTRING_SIZE 32
#define MAXSYMBOL_SIZE 512

// define **********************************************************

typedef struct his_i
{
	int mark;  // Whether the tag is valid information   >=0 : invalid  -1: effective
	char time[21];
	char command[MAXCOMMAND_SIZE];
}history_info_type;

typedef struct his_t
{
	int size;
	history_info_type history_info[1024];
}history_table_type;

//extern **********************************************************

int inithisfiledir();
int initialize_history_table();
int open_and_creat();
int close_hisfile();
int get_last_history(command_type* pdest);
int get_next_history(command_type* pdest);
int add_to_history(command_type* pcommand);
int add_and_clearchoice(command_type* pcommand);
int get_nubmer(int fd);
int write_history(int fd,int hisbegin,int hisend);   // include hisend
int get_nextnumber(int fd);   // get next number at file
int goto_buffer_offest(int fd,int numberofhis,int* pbegin);
	// begin is relative to the current position
int read_file_to_histable(int fd, int beginnum ,int endnum);  // no error judge
int history_moveto_head(int beginnum ,int endnum);   
	// Move the command of the selected area to the head     include endnum
int close_history();
int print_histable(int beginnum ,int endnum); //include endnum
int print_hisinfo(history_info_type* phis);
int updatenumber();
int get_number_string(char* string,int number);
int inti_history_env();
int keephistory_fd_at(int his_fd, int number);
int historyfile_clear();
int history_print(int beginnum,int endnum);
int command_history(int argc,char* arg[]);
int analysis_hissymbol(char* string,char* sret);
int get_hissymbol(char* command_content, char* string); 

extern history_table_type history_table;
extern char* historyfilename;
extern directory_type historyfiledir;
extern envtable history_envtable;



#endif