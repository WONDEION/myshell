#include "command_type.h"

static int distype_switch = 0;
static int disdir_switch = 0;
static int disname_switch = 0;
static int dis_all = 0;
static char type_type[32] = {'\0'}; 
static char type_word[MAXCOMMAND_SIZE];
static char type_value[MAXCOMMAND_SIZE];


static int setmode_t();
static int setmode_p();
static int setmode_a();
static int setmode_normal();

int shellcommand_type(int argc, char* arg[])
{
	int i = 0;
	int i_p = 0;
	int i_t = 0;
	int i_a = 0;
	int* mark = NULL;

	if(0 == argc || NULL == arg)
	{
		return -1;
	}

	if(strcmp("type",arg[0]))
	{
		shellcommandNOcall(STDERR_FILENO,"type");
		return -1;
	}
	mark = (int*)malloc(sizeof(int) * argc);
	mark[0] = 1;
	// normal state
	setmode_a();	
	//
 	if(-1 != (i_t = find_char_from_arg(argc,arg,'t')))
 	{
 		mark[i_t] = 1;
 		setmode_t();
 	}
 	if(-1 != (i_p = find_char_from_arg(argc,arg,'p')))
 	{
 		mark[i_p] = 1;
 		setmode_p();
 	}
 	if(-1 != (i_a = find_char_from_arg(argc,arg,'a')))
 	{
 		mark[i_a] = 1;
 		setmode_a();
 	}
 	for(i = 1; arg[i] != NULL ; i++)
 	{
 		if(i_p == i || i_t == i || i_a == i)
 		{
 			continue ;
 		}
 		// alias
 		mark[i] = 1;
 		if(isalias(arg[i]))
 		{
 			strcpy(type_word,arg[i]);
 			strcpy(type_value,arg[i]);
 			develop_alias(type_value);
 			strcpy(type_type,"alias");
 			type_display();
 			if(0 == dis_all)
 			{
 				return 0;
 			}
 		}
 		if(-1 != isshellcommand(arg[i]))
 		{
 			strcpy(type_word,arg[i]);
 			strcpy(type_type,"shell command");
 			strcpy(type_value,"");
 			type_display();
 			if(0 == dis_all)
 			{
 				return 0;
 			}
 		}
 		if(islinuxcommand(arg[i]))
 		{
 			strcpy(type_word,arg[i]);
 			strcpy(type_type,"Linux command");
 			get_linuxcommand_dir(arg[i], type_value);
 			type_display();
 			if(0 == dis_all)
 			{
 				return 0;
 			}
 		}
 	}
 	if(Nooptions(STDERR_FILENO, "type", mark, arg))
 	{
 		free(mark);
 		return 1;
 	}
 	free(mark);
 	return 0;
}

static int setmode_t()
{
	distype_switch = 1;
	dis_all = 0;
	disdir_switch = 0;
	disname_switch = 0;
	return 0;
}

static int setmode_p()
{
	distype_switch = 0;
	dis_all = 0;
	disdir_switch = 1;
	disname_switch = 0;
	return 0;
}

static int setmode_a()
{
	distype_switch = 1;
	dis_all = 1;
	disdir_switch = 1;
	disname_switch = 1;
	return 0;
}

static int setmode_normal()
{
	distype_switch = 0;
	dis_all = 0;
	disdir_switch = 1;
	disname_switch = 1;
	return 0;
}

int type_display()
{
	if(disname_switch)
	{
		printf("%s is ",type_word);
	}
	if(distype_switch)
	{
		printf("%s ",type_type);
	}
	if(disdir_switch)
	{
		printf("%s", type_value);
	}
	printf("\n");
	return 0;
}