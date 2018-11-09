
#ifndef COMMANDLET_H
#define COMMANDLET_H

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "Datastruct.h"
#include "selflib.h"
#include "misstake.h"
#include "process.h"

// define
typedef struct exp_info_t
{
	int value;
	char type;
	char ope[3];
}expression_info_type;

typedef struct exp_t
{
	expression_info_type express[MAXCOMMAND_SIZE];
	int size;
}expression_type;

typedef struct lv_c
{	
	char name[MAXNAME_SIZE];
	char ope[3];
}leftvalue_container_type;

int command_let(int argc , char* arg[]);
int analusis_cal(char* calculation,int strleng,int* local, int* pvalue);
int get_expressnumber(char* string,int *local, int strleng);
int iscalculate(char* expression,int strleng);
int isassignment(char* expression,int strleng);
int assignmenttype_and_local(char* string, int strleng, int* local,char *ope);
int iscompare(char* expression,int strleng);
int comparetype_and_local(char* string, int strleng, int* local,char *ope);  // return type
int resolve_assignment(char* string, int strleng, int* pvalue);
int compute(char* expression,int calleng,int* pvalue);
int let_resolve(char* expression, int strleng, int* local, int* pvalue);
int let_get_number(char* string, int* local, int* pvalue);
int let_get_operator(char* string, int* local, char* poperator);
int infix_to_postfix(expression_type* expdest,expression_type* expsource);
int calculator_postfix(expression_type* expression, int* pvalue);
int calculator_value(int value_1, char* ope, int value_2, int* pvalue);
int compara_operator(char* ope1, char* ope2);  //  ope1 > ope2  : 2
int operator_priority(char* ope); // 1, 2, 3, 4, 5, ,6, 7, 8, 9
int develop_envselfadd(char* string,int strleng, int* local, int* pleng);
int is_expressionlegality(char* expression, int mode);
int is_calculatorchar(char ch);
int expression_copy(expression_type* expdest, expression_type* expsource);
int expression_info_copy(expression_info_type* expdest, expression_info_type* expsource);
int isunaryoperator(char ch);
int pushexpression_d(expression_type* pexp,int digit,char* ope,char type);
int popexpression_d(expression_type* pexp,int* pdi, char* ope, char* ptype);
int pushexpression(expression_type* pexp,expression_info_type* pinfo);
int popexpression(expression_type* pexp,expression_info_type* pinfo);
int printexpression(expression_type* expression);

#endif