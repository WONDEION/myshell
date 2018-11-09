
 
#ifndef DATASTRUCHT_H

#define DATASTRUCHT_H
	


//include ***************************************************************************

#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <termio.h>
#include <signal.h>  
#include <malloc.h>
#include <sys/ioctl.h>
#include <string.h>
#include <limits.h>
#include <sys/resource.h>   // getlimit
//#include "charanalysis.h"

#include "selflib.h"

//***************************************************************************



//def***************************************************************************
#define MAXCOMMAND_SIZE 8192
#define DIRECTORY_SIZE 8192
#define MAXNAME_SIZE NAME_MAX
#define MAXENV_VALUE_SIZE 512
#define MAXSTACK_SIZE 512
#define MAXCOMMAND_LINE 512
#define MAXTERMIO_SIZE 512
#define MALLOC_SIZE 1024
#define MAXENV_SIZE 16384
//******************************************************************************


// datastruct class define ***************************************************************************

typedef struct position_type
{
	int x;
	int y;
}position;

typedef struct com
{
	char command_content[MAXCOMMAND_SIZE];  // from readline.h 
	int size;   // number of char int commmand
	int line;	// number of line
	int linesize[MAXCOMMAND_LINE];
	int screenline; // number of screenline
	int slinesize[MAXCOMMAND_LINE];
	int cursorlocal;	// the position of the cursor relative to the linesize    (array mode)s
	position line_position;    // x,y is for line
	position cursor_position;   // x,y is for screenline
	int pushstack_times;  // The number of states saved
}command_type;

typedef int (*charag_functiontable_type)(command_type*);

typedef struct key_value
{
	int value;
	char key[MAXNAME_SIZE];
}key_value;

typedef struct command_stack_type
{
	command_type stack_body[MAXSTACK_SIZE];
	int sp;
}command_stack;

typedef struct stack_type
{
	int sp;
	int type_size;
	void* (content[MAXSTACK_SIZE]);
}stack;

typedef struct charstack_type
{
	int sp;
	char content[MAXSTACK_SIZE];
}stack_char_type;

typedef struct intstack_type
{
	int sp;
	int content[MAXSTACK_SIZE];
}stack_int_type;

typedef struct dir
{
	char directory[DIRECTORY_SIZE];
}directory_type;

typedef struct ev
{
	char name[MAXNAME_SIZE];
	char value[MAXENV_VALUE_SIZE];
}environment_variable;

typedef struct evt
{
	int number;  // choise where add
	int size;  // all size of ptalbe
	environment_variable* ptable;
}envtable;

typedef struct M_t
{
	void* menory_table;
	int* size_byte;
	int size;
}Memory_table;

typedef struct proc_t
{
	pid_t pid;
	uid_t uid;
	char name[MAXCOMMAND_SIZE];
	int state;   // 1 run 2 backrun 0 stop -1 quit(Exiting,will delete infomation if process have exited )  
	char** arg;
}process_info_type;

typedef struct proc_table
{
	process_info_type* process_info;
	int* mark;
	int end;
}process_table_type;

//******************************************************************************


// datastruct define***************************************************************************

//  extern ******

//stack
int clear_commandstack(command_stack* pstack);
int push_command(command_stack* pstack,command_type* pcommand);
int pop_command(command_stack* pstack,command_type* pcommand);
int push(stack* pstack,void* source);
int pop(stack* pstack,void* dest);
int push_char(char ch);
char pop_char();
int push_int(stack_int_type* pstack, int digit);
int pop_int(stack_int_type* pstack,int* pdigit);
// command
int combinecommand(command_type* paim, command_type* psource);
int command_copy(command_type* pdest,command_type* psource);
int clearcommand(command_type*);
int clearline(command_type* pcommand);
int addiscode_to_command(command_type *pcommand,char inputchar);
int printchar(command_type* pcommand,char inputchar);
int write_and_back(int fd, command_type* pcommand, char* string); 
int translational_cursor(int num,int dre);  // 1 left  2 right  3 up 4 down 
int display_linehead();
int updatecursor(command_type* pcommand , int newlocal);  // Update the position of the cursor on the screen by command
int printf_command_withcursor(command_type* pcommand);
	// original representing the original position, cursorlocal representin the new position
int setsrceen_from_size_cursor(command_type* pcommand);  // set screen from command size and cursirlacol
int setsrsize_from_size(command_type* pcommand);
int updateother(command_type* pcommand);  // change other form size and cursor
//directory
int dir_assignment(directory_type* pdir ,char* string_dir);
int dir_copy(directory_type* pdest, directory_type* psource);
int dir_cat(directory_type* pfirst, directory_type* psecond);
int dir_catstring(directory_type* pdir, char* string);
int delete_lastlv_dir(directory_type* pdir);
int delete_lastlv_dir_string(char* dir);
//
int is_brace_matching(char* string);
int isbrace(char ch);
int is_quotes_match(char* string);
int is_commandspacer(char* command);
//
char** malloc_arg(int n);
int free_arg(char** arg);

extern command_stack stack_command;     // command stack for save command state and revoke
extern stack_char_type stack_char;
extern stack_int_type stack_int;
extern char begindir[DIRECTORY_SIZE];
extern char nowdir[DIRECTORY_SIZE]; 
extern char programdir[DIRECTORY_SIZE];
extern struct passwd *user_inf;
extern struct winsize windows_info;
extern int dirchange;
extern int disline_dirloc;
extern directory_type home;
extern struct passwd* user_inf;
extern directory_type workdir;
extern uid_t uid;
extern command_type* nowcommand_point;
extern environment_variable user_bgdir;
extern envtable msh_envtable;
extern int display_open;

extern char disline_normal[DIRECTORY_SIZE + MAXNAME_SIZE] ;
extern char disline_again[2];
extern char disline[DIRECTORY_SIZE + MAXNAME_SIZE];
extern int disline_realleng;

// state variable
extern int on_read; // 1 is in readline
extern int bg_lock;
extern int due_char;
extern int havesignal;

//***************************************************************************

//extern other file

//***************************************************************************



#endif