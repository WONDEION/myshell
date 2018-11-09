#include "shellcommand.h"

char* shellcommand_nametable[] = {"history","cd","source","alias","unalias",
								  "jobs","fg","bg","type","exit","let",NULL};
int (*(shellcommand_functiontable[]))(int,char**) = {command_history,command_cd,
													command_source,command_alias,
													command_unalias,command_jobs,
													command_fg,command_bg,
													shellcommand_type,command_exit,
													command_let,};

int devecalculation(char* command)  // $(())
{
	int i = 0;
	int j = 0;
	int value = 0;
	int org_leng = 0;
	int begin = 0;
	int value_leng = 0;
	int bracemark = 0;
	char value_string[MAXCOMMAND_SIZE];
	char bufcommand[MAXCOMMAND_SIZE];
	if(NULL == command)
	{
		return -1;
	}
	for(i = 0;'\0' != command[i]; i++)
	{
		if('\'' == command[i])
		{
			i++;
			while('\'' != command[i] && '\0' != command[i])
			{
				i++;
			}
			if('\0' == command[i])
			{
				return 0;
			}
		}
		else
		{
			if('$' == command[i])
			{
				begin = i;
				if('(' == command[i + 1] && '(' == command[i + 2])
				{
					// compute
					if(-1 == exec_computecommand(command + i + 1,&value))
					{
						return -1;
					}
					// get org_leng
					i = i + 3;
					bracemark = 0;
					while('\0' != command[i])
					{
						if(0 == bracemark && ')' == command[i])
						{
							//printf("here\n");
							break;
						}
						if(0 != bracemark && ')' == command[i])
						{
							bracemark--;
						}
						if('(' == command[i])
						{
							bracemark++;
						}
						i++;
					}
					if('\0' == command[i] || ')'!= command[i + 1])
					{
						//rintf("here\n");
						return -1;
					}
					i = i + 2; // step  
					org_leng = i - begin;
					// get value leng;
					int_to_string(value,value_string,10);
					value_leng = strlen(value_string);
					// set command
					//printf("value_leng : %d| org_leng : %d| i : %d|\n", 
					//			value_leng,org_leng,i);
					string_translation(command,i,value_leng - org_leng);
					for(j = begin; j < value_leng; j++)
					{
						command[j] = value_string[j];
					}
					//set i
					i = j;
				}
			}
		}
	}
}

int isshellcommand(char* command)
{
	int i = 0;
	char comname[MAXCOMMAND_SIZE];
	if(NULL == command)
	{
		return -1;
	}
	get_first_word(command, comname);
	for(i = 0 ; shellcommand_nametable[i] != NULL ; i++)
	{
		if(!strcmp(comname,shellcommand_nametable[i]))
		{
			return i;
		}
	}
	return -1;
}

int gotoshellcommand(char* command)
{
	int i = 0;
	int argc = 0;
	char** arg = NULL;
	int state = 0;
	i = isshellcommand(command);
	if(i == -1)
	{
		return -1;
	}
	argc = get_string_words(command);
	arg = malloc_arg(argc);
	if(arg == NULL)
	{
		printf("malloc_arg error !!\n");
		return -1;
	}
	string_to_arg(command, arg); // no malloc 
	//exec 
	state = shellcommand_functiontable[i](argc,arg);
	free_arg(arg);
	return state;
}

int isbarcecommand(char* command)  // (())  []   [[]]
{
	if(NULL == command)
	{
		return 0;
	}
	// (())
	if('(' == command[0] && '(' == command[1] && 
		')' == command[strlen(command) - 2] && ')' == command[strlen(command) - 1])
	{
		return 1;
	}
	// [[]]
	if('[' == command[0] && '[' == command[1] && 
		']' == command[strlen(command) - 2] && ']' == command[strlen(command) - 1])
	{
		return 1;
	}
	return 0;
}

int gotobracketscommand(char* command)
{
	int value = 0;
	if(NULL == command)
	{
		return -1;
	}
	if('[' == command[0] && '[' == command[1] && 
		']' == command[strlen(command) - 2] && ']' == command[strlen(command) - 1])
	{
		
	}
	if('(' == command[0] && '(' == command[1] && 
		')' == command[strlen(command) - 2] && ')' == command[strlen(command) - 1])
	{
		if(-1 == exec_computecommand(command,&value))
		{
			return -1;
		}
		return 0;
	}
	return 0;
}

int exec_computecommand(char* command, int* pvalue)
{
	int i = 0;
	int value = 0;
	int bracemark = 0;
	char bufcommand[MAXCOMMAND_SIZE];

	if(NULL == command || NULL == pvalue)
	{
		return -1;
	}
	if('(' != command[0] || '(' != command[1])
	{
		return -1;
	}
	// get compute epression
	for(i = 0; '\0' != command[2 + i] ; i++)
	{
		if(0 == bracemark && ')' == command[2 + i])
		{
			break;
		}
		if(')' == command[2 + i] && 0 != bracemark)
		{
			bracemark--;
		}
		if('(' == command[2 + i])
		{
			bracemark++;
		}
		bufcommand[i] = command[2 + i];
	}
	bufcommand[i] = '\0';
	//
	if( bracemark != 0)
	{
		return -1;
	}
	// exec
	i = 0;
	clear_space(bufcommand);
	analusis_cal(bufcommand,strlen(bufcommand),&i, &value);
	*pvalue = value;
	return 0;
}