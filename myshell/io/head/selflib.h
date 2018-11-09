#ifndef SELFLIB_H
#define SELFLIB_H

//include
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "misstake.h"
//#include "charanalysis.h"

//

//define 

#define CACHE_SIZE 4096

//

typedef struct myprint_inf
{
	int width;
	int accuracy;
	int align;   // 1 (-)   2 (+)  4 ( ) 8(#)
	int fill;    // 1 ' '   0 '0'
}myprint_inf; 

//extren
char msh_getchar(int fd);
char getfromcache();
int addchar_lockcache(char inputchar);
int string_translation(char* string,int loc,int direction);    // 0< left   0> right
int get_int_bit(int digit);
int issthex(char digit_bit);
int get_first_word(char* string, char* word);
int find_word_from_string(char* string, char* word);
int get_word_times(char* string,char* word);
int string_to_arg(char* string, char** arg); // no malloc 
int arg_to_string(char* arg[],char* string);
int get_string_words(char* string);
int string_remove_word(char* string, char* word);
int clear_intarray(int* ar, int n);
int int_to_string(long int target, char* string, int hexadecimal);
int isalldigit(char* string);
int clear_space(char* string);

int msh_fprintf(int fd,char* format, ...);
int mprint_getmark(char* format,int* p_iformat); // return myprint_inf.align     '%' has been skip
int append_s(char* pdest, int* p_idest , char* psource, myprint_inf* ptype);  // retrun number of characters in the actual copy
int append_c(char* pdest, int* p_idest , char inputchar, myprint_inf* ptype);
int append_d(char* pdest, int* p_idest , int inputint, myprint_inf* ptype); // retrun number of characters in the actual copy
int append_u(char* pdest, int* p_idest , unsigned int inputunint, myprint_inf* ptype);
int deal_acc_and_width(char* string, char* paim, myprint_inf* ptype,char*);
int append_X(char* pdest, int* p_idest , unsigned int inputox, myprint_inf* ptype);
int append_x(char* pdest, int* p_idest , unsigned int inputox, myprint_inf* ptype);
int append_o(char* pdest, int* p_idest , unsigned int inputo, myprint_inf* ptype);
int append_e();
int append_E();
int append_g();
int append_f();
int append_l();
int append_lf();

int print_arg(int argc, char* arg[]);
int findnum_from_arg(int argc,char* arg[]);
int findchar_from_arg(int argc,char* arg[],char c);   // char
int findstring_from_arg(int argc,char* arg[],char* string);	// string
int find_num_from_arg(int argc,char* arg[]);   // -int
int find_char_from_arg(int argc,char* arg[],char c);	// -char
int find_string_from_arg(int argc,char* arg[],char* string);	// -string

//
char command_lockcache[CACHE_SIZE]; 

extern int printf_fd; 

#endif