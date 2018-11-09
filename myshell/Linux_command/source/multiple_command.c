#include "multiple_command.h"

int iscommandgroup(char* command)
{	
	int i = 0;
	if(NULL == command)
	{
		return -1;
	}
	if('{' == command[0] && 
	    '}' == command[strlen(command) - 1])
	{
		return 1;
	}
	if('(' == command[0] && '(' != command[1]
		&& ')' == command[strlen(command) - 1]
		&& ')' != command[strlen(command) - 2])
	{
		return 1;
	}
	for(i = 0 ; command[i] != '\0' ; i++)
	{
		if('\'' == command[i] || '\"' == command[i])
		{
			int ch = 0;
			ch = command[i];
			i++;
			while(ch != command[i])
			{
				i++;
			}
			// needn't step
			continue;
		}
		if(';' == command[i])
		{
			return 1;
		}
	}
	return 0;
}

int gotocommandgroup(char* command)
{
	int state = 0;
	if(NULL == command)
	{
		return 0;
	}
	if('(' == command[0] && ')' == command[strlen(command) - 1])
	{
		state = build_commandgroup(command);
		return state;
	}
	else if('{' == command[0] && '}' == command[strlen(command) - 1])
	{
		state = nobuild_commandgroup(command);
		return state;
	}
	else  // ~~ ; ~~ ; ~~
	{
		state = commandgroup(command);
		return state;
	}
}

int build_commandgroup(char* command)
{
	int argc = 0;
	char** arg = NULL;
	int wait = 1;
	int i = 0;
	if(NULL == command)
	{
		return -1;
	}
	if(-1 != find_word_from_string(command, "&"))
	{
		fatherwait = 0;
	}
	// get argv
	argc = 1 + 1 + 1 + 1;
	arg = malloc_arg(argc);
	strcpy(arg[0],programdir);
	//strcat(arg[0],"/myshell");
	//printf("arg[0] : %s\n",arg[0]);
	strcpy(arg[1],"-go");
	strcpy(arg[2],command + 1);
	for(i = strlen(arg[2]) - 1 ;i >= 0; i--)
	{
		if(')' == arg[2][i])
		{
			arg[2][i] = '\0';
		}
	}
	arg[3] = NULL;
	//exec
	//print_arg(argc,arg);
	fork_exec(programdir,"myshell", argc, arg);

	if(fatherwait)
	{ 
		free_arg(arg);
	}
	return 0;
}

int nobuild_commandgroup(char* command)
{
	char temp[MAXCOMMAND_SIZE];
	if(NULL == command)
	{
		return -1;
	}
	strcpy(temp,command + 1);
	temp[strlen(temp) -1 ] ='\0';
	commandgroup(temp);
	return 0;
}

int commandgroup(char* command)
{
	int i = 0;
	int j = 0;
	int contin = 0;
	char singlecommand[MAXCOMMAND_SIZE];
	char ch;

	if(NULL == command)
	{
		return 0;
	}
	if(0 == strlen(command))
	{
		return 0;
	}
	i = 0;
	j = 0;
	contin = 1;
	while(contin)
	{
		j = 0;
		while(';' != command[i] && '\0' != command[i])
		{
			if('\'' == command[i] || '\"' == command[i])
	    	{
	    		ch = command[i];
	    		i++;
	    		singlecommand[j] = command[i];
				i++;
				j++;
	    		while(ch != command[i])
	    		{
	    			singlecommand[j] = command[i];
					i++;
					j++;
	    			i++;
	    		}
	    		singlecommand[j] = command[i];
				i++;
				j++;
	    	}
	    	else
	    	{
	    		singlecommand[j] = command[i];
				i++;
				j++;
	    	}
		}
		singlecommand[j] = '\0';
		if('\0' == command[i])
		{
			contin = 0;
		}
		else
		{
			i++; //step ';'
		}
		// due 
		if( -1 == process_input(singlecommand))
		{
			continue;
		}
		//run
		close_all_switch();
		analysis_command_primary(singlecommand);
		open_all_switch();
	}
	

	return 0;
}

int isqueuecommand(char* command)
{
	int i = 0;
	if (NULL == command)
	{
		return 0;
	}

	for(i = 0; command[i] != '\0'; i++)
	{
		if('\'' == command[i] || '\"' == command[i])
		{
			int ch = 0;
			ch = command[i];
			i++;
			while(ch != command[i])
			{
				i++;
			}
			// needn't step
			continue;
		}
		if(command[i] == '&' || command[i] == '|')
		{
			if(command[i] == command[i + 1])
			{
				return 1;
			}
		}
	}
	return 0;
}

int gotoqueuecommand(char* command)
{
	write(STDOUT_FILENO,"queue command\n",15);
	return execqueuecommand(command);
}

int execqueuecommand(char* command)  // 0 means normal  1 means unnormal
{
	int i = 0;
	int j = 0;
	char buf[MAXCOMMAND_SIZE];
	int command_leng = strlen(command);
	int leftstate = 0; // 1 success -1 falied
	int nowstate = 0;
	int times = 0;
	char conmode = 0; // & , |
	char lastconmode = 0;
	char ch;
	if(NULL == command)
	{
		return -1;
	}
	i = 0;
	while(i < command_leng)
	{
		j = 0;
		while('\0' != command[i] && !is_orwithspacer(command + i))
		{
			if('\'' == command[i] || '\"' == command[i])
	    	{
	    		ch = command[i];
	    		i++;
	    		buf[j] = command[i];
				i++;
				j++;
	    		while(ch != command[i])
	    		{
	    			buf[j] = command[i];
					i++;
					j++;
	    			i++;
	    		}
	    		buf[j] = command[i];
				i++;
				j++;
	    	}
	    	else
	    	{
	    		buf[j] = command[i];
				i++;
				j++;
	    	}
		}
		buf[j] = '\0';
		// run
		process_input(buf);
		close_all_switch();
		nowstate = analysis_command_primary(buf);
		open_all_switch();
		//printf("nowstate : %d\n",nowstate);
		// get Connection mode
		if('\0' != command[i])
		{
			if('&' == command[i])
			{
				conmode = '&';
			}
			else if('|' == command[i])
			{
				conmode = '|';
			}
			i = i + 2;
		}
		//printf("conmode : %c\n", conmode);
		//  judge 
		if(0 == times)
		{
			if('&' == conmode && 0 != nowstate) 
			{
				return 1;
			}
			leftstate = nowstate;
			times = 1;
		}
		else
		{
			if('&' == lastconmode)
			{
				if(leftstate != 0 || nowstate != 0)  // match failed
				{
					if('&' == conmode)  // if next is "&&"
					{
						return 1;
					}
					else
					{
						leftstate = 1;
					}
				}
				else
				{
					leftstate = 0;
				}
			}
			else if('|' == lastconmode)
			{
				if(leftstate != 0 && nowstate != 0) // match failed
				{
					if('&' == conmode)  // if next is "&&"
					{
						return 1;
					}
					else
					{
						leftstate = 1;
					}
				}
				else
				{
					leftstate = 0;
				}

			}
		}
		lastconmode = conmode;
	}
	return 0;
}

int is_orwithspacer(char* pchar)
{
	if(NULL == pchar)
	{
		return 0;
	}
	if('&' == pchar[0] || '|' == pchar[0])
	{
		if(pchar[0] == pchar[1])
		{
			return 1;
		}
	}
	return 0;
}

