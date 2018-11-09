#include "command_jobs.h"


// normal number state name
// -p pid
// -l number pid name
// -r run command
// -s 

//display switch
static int dis_number = 0;
static int dis_pid = 0;
static int dis_state = 0;
static int dis_command = 0;
static int dis_runcom = 0;
static int dis_stopcom = 0;

int command_jobs(int argc, char* arg[])
{
	int i = 0;
	int* mark = NULL;
	char statestring[30];

	if(NULL == arg || 0 == argc)
	{
		return -1;
	}

	if(strcmp("jobs",arg[0]))
	{
		shellcommandNOcall(STDERR_FILENO,"jobs");
		return -1;
	}
	// set normal switch
	dis_number = 1;
	dis_state = 1;
	dis_command = 1;
	dis_pid = 0;
	dis_runcom = 1;
	dis_stopcom = 1;
	// change 
	if(-1 != (i = find_char_from_arg(argc,arg,'p')))  // -p and -l cannot coexist
	{
		dis_number = 0;
		dis_state = 0;
		dis_command = 0;
		dis_pid = 1;
	}
	else if(-1 != (i = find_char_from_arg(argc,arg,'l')))
	{
		dis_pid = 1;
	}
	if(-1 != (i = find_char_from_arg(argc,arg,'r')))
	{
		dis_stopcom = 0;
	}
	if(-1 != (i = find_char_from_arg(argc,arg,'s')))
	{
		dis_runcom = 0;
	}
	// 
	for(i = 0; i < process_table.end;i++)
	{
		if(0 == dis_runcom)
		{
			if(1 == process_table.process_info[i].state 
				|| 2 == process_table.process_info[i].state)
			{
				continue;
			}
		}
		if(0 == dis_stopcom)
		{
			if(0 == process_table.process_info[i].state)
			{
				continue;
			}
		}
		if(dis_number)
		{
			printf("[%d] ",i + 1);
		}
		if(dis_pid)
		{
			printf("%d ",process_table.process_info[i].pid);	
		}
		if(dis_state)
		{
			if(1 == process_table.process_info[i].state
				|| 2 == process_table.process_info[i].state)
			{
				strcpy(statestring,"Running");
			}
			if(0 == process_table.process_info[i].state)
			{
				strcpy(statestring,"Stop");
			}
			printf("%s", statestring);
		}
		if(dis_command)
		{
			printf("                  %s",process_table.process_info[i].name);
		}
		printf("\n");
	}

	if(Nooptions(STDERR_FILENO, "jobs", mark, arg))
	{
		free(mark);
		return 1;
	}
	free(mark);
	return 0;
}

int command_fg(int argc, char* arg[])
{
	int i = 0;
	int* mark = NULL;
	int number = 0;
	int state = 0;
	if(0 == argc || NULL == arg)
	{
		return 0;
	}
	if(strcmp("fg",arg[0]))
	{
		shellcommandNOcall(STDERR_FILENO,"fg");
		return -1;
	}	
	mark = (int*)malloc(argc * sizeof(int));
	mark[0] = 1;
	for(i = 1; NULL != arg[i] ; i++)
	{
		if(isalldigit(arg[i]))  // parameter  legitimate 1
		{
			number = atoi(arg[i]);
			if(number != 0 && number <= process_table.end)  // parameter  legitimate 2
			{
				mark[i] = 1;
				//printf("number : %d\n", number);
				number--;
				change_process_state(process_table.process_info[number].pid,1);
				set_run_termode();
				fatherwait = 1;
				//wait_spid = process_table.process_info[number].pid;
				kill(process_table.process_info[number].pid,SIGCONT);
				waitpid(process_table.process_info[number].pid,&state ,WUNTRACED);
				if(WIFSTOPPED(state))
				{
					//printf("is stop\n");
					change_process_state(process_table.process_info[number].pid,0);
				}
				else if(WIFEXITED(state) || WIFSIGNALED(state))
				{
					delete_proinfo_pid(process_table.process_info[number].pid);
				}
				//wait_spid = -1;
				setmode();
				fatherwait = 0;
			}
		}
	}
	if(Nooptions(STDERR_FILENO,"fg",mark,arg))
	{
		free(mark);
		return 1;
	}	
	free(mark);	
	return 0;
}

int command_bg(int argc, char* arg[])
{
	int i = 0;
	int* mark = NULL;
	int number = 0;
	if(0 == argc || NULL == arg)
	{
		return 0;
	}
	if(strcmp("bg",arg[0]))
	{
		shellcommandNOcall(STDERR_FILENO,"bg");
		return -1;
	}
	mark = (int*)malloc(argc * sizeof(int));
	mark[0] = 1;
	for(i = 1; NULL != arg[i] ; i++)
	{
		if(isalldigit(arg[i]))  // parameter  legitimate 1
		{
			number = atoi(arg[i]);
			if(number != 0 && number <= process_table.end)  // parameter  legitimate 2
			{
				if(0 != process_table.process_info[number].state)
				{
					continue;
				}
				mark[i] = 1;
				number--;
				kill(process_table.process_info[number].pid,SIGCONT);
				bg_lock = 1;
				/*while(!havesignal && bg_lock)
				{
					;
				}
				havesignal = 0;*/
			}
		}
	}
	if(Nooptions(STDERR_FILENO,"bg",mark,arg))
	{
		free(mark);
		return 1;
	}
	free(mark);
	return 0;
}

int command_kill(int argc, char* arg[])
{
	int i = 0;
	int* mark = NULL;
	if(0 == argc || NULL == arg)
	{
		return 0;
	}
	if(0 == argc || NULL == arg)
	{
		return 0;
	}
	if(strcmp("kill",arg[0]))
	{
		shellcommandNOcall(STDERR_FILENO,"kill");
		return -1;
	}
	mark = (int*)malloc(argc * sizeof(int));
	
	Nooptions(STDERR_FILENO,"",mark,arg);
	free(mark);
	return 0;
}