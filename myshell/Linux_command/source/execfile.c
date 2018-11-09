#include "execfile.h"

int isexecfile(char* command)
{
	char filename[DIRECTORY_SIZE];

	if(NULL == command)
	{
		return 0;
	}

	if( -1 == get_first_word(command,filename))
	{
		//printf("isexecfile : get_first_word error!!\n");
		return 0;
	}
	if( -1 == develop_abbdir(filename))
	{
		//printf("isexecfile : develop_abbdir error!!\n");
		return 0;
	}
	if(testfile(filename))
	{
		return 1;
	}
	return 0;
}

int testfile(char* filename)
{
	int i = 0;
	struct stat rest;
	if(NULL == filename)
	{
		return 0;
	}
	if(-1 == (i = stat(filename,&rest)))
	{
		
		if(errno ==  ENOENT)
		{
			printf("No file :\"%s\"\n", filename);
		}		
		return 0;
	}
	//printf("i %d\n", i);
	if(!S_ISREG(rest.st_mode))
	{
		printf("File \"%s\" is not executable file\n", filename);
		return 0;
	}
	return 1;
}

int execfile(char* command)
{
	int i = 0;
	int state = 0;
	char filename[MAXCOMMAND_SIZE];
	char dir[DIRECTORY_SIZE];
	int argc = 0;
	char** arg = NULL;
	
	if(NULL == command)
	{
		return -1;
	}
	// get file dir
	get_first_word(command,filename);
	strcpy(dir,filename);
	develop_abbdir(dir);
	adjustfilename(filename);  // remove '/'
	//
	if(-1 != find_word_from_string(command,"&"))
	{
		//printf("no wait\n");
		string_remove_word(command,"&");
		fatherwait = 0;
	}
	else
	{
		fatherwait = 1;
	}
	argc = get_string_words(command);
	// mallco arg
	arg = malloc_arg(argc);
	// set filename 
	string_to_arg(command,arg);
	strcpy(arg[0],filename);
	// fork
	state = fork_exec(dir,command,argc,arg);

	return state;
}


int fork_exec(char* dir, char* command, int argc, char** arg)
{
	pid_t s_pid = -1;
	int state = 0;
	int retstate = 0;
	int exitstate = 0;

	if(NULL == dir || NULL == command || 0 == argc || NULL == arg)
	{
		return -1;
	}
	sonwait = 1;
	if((s_pid = fork()) < 0)
	{
		printf("fork error !!\n");
	}
	else if(s_pid == 0)  // son
	{
		//setgroup(s_pid,fatherwait);
		// set signal
		while(sonwait)
		{
			;
		}
		//exec
		if(1 == fatherwait)
		{
			set_run_termode();
		}
		if(-1 == execv(dir,arg))
		{
			printf("File execv error!! dir : %s\n",dir);
		}
		exit(0);
	}
	else  // father
	{
		if(1 == fatherwait)
		{
			//wait_spid = s_pid;
			addto_processtable_dispersed(command,s_pid,1,arg);
			kill(s_pid,SIGUSR1);
			waitpid(s_pid,&state,0 | WUNTRACED);
			if(WIFSTOPPED(state))
			{
				//printf("is stop\n");
				change_process_state(s_pid,0);
				retstate = 0;
			}
			else // quit
			{
				if(WIFEXITED(state))
				{
					delete_proinfo_pid(s_pid);
					exitstate = WEXITSTATUS(state);
					//printf("exitstate : %d\n", exitstate);
					retstate = exitstate;
				}
				if(WIFSIGNALED(state))
				{
					delete_proinfo_pid(s_pid);
					retstate = -1;
				}
			}
			//wait_spid = -1;
			setmode();
		}
		else
		{
			//add to process
			addto_processtable_dispersed(command,s_pid,2,arg);
			printf("[%d] %s\n",s_pid,command);
			kill(s_pid,SIGUSR1);
		}
	}
	fatherwait = 0;
	setmode();
	return retstate;
}
int adjustfilename(char* filename)
{
	int i = 0;
	if(NULL == filename)
	{
		return -1;
	}
	for(i = strlen(filename) - 1; i >= 0; i--)
	{
		if( '/' == filename[i])
		{
			break;
		}
	}
	if(i == -1)
	{
		return 0;
	}
	string_translation(filename,i + 1, -( i + 1 ));
	return 0;
}