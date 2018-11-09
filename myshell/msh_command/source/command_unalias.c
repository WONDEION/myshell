#include "command_unalias.h"

int command_unalias(int argc , char* arg[])
{
	int i = 0;
	int* mark = NULL;
	int retstate = 0;

	if(0 == argc || NULL == arg)
	{
		return -1;
	}

	mark = (int*)malloc(argc * sizeof(int));
	if(strcmp("unalias",arg[0]))
	{
		shellcommandNOcall(STDERR_FILENO,"unalias");
		return -1;
	}
	mark[0] = 1;
	if(-1 != (i = find_char_from_arg(argc,arg,'a')))
	{
		delete_allalias();
		mark[i] = 1;
		free(mark);
		return 0;
	}
	for(i = 1; arg[i] != NULL ; i++)
	{
		if(-1 == deletenv_name(&alias_table, arg[i]))
		{
			printf("No alias \"%s\"!!\n",arg[i]);
			retstate = 1;
		}
		else
		{
			mark[i] = 1;
			ifchange_table = 1;
		}
	}

	if(Nooptions(STDERR_FILENO, "unalias", mark, arg))
	{
		retstate = 1;
	}
	free(mark);
	return retstate;
}

int delete_allalias()
{
	ifchange_table = 1;
	alias_table.number = 0;
	return 0;
}