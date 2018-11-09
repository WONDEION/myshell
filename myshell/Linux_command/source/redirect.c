#include "redirect.h"

int isredirect(char* command)   //-1 error 0 no redirect 1 have redirect
{
	int i  = 0;
	if(NULL == command)
	{
		return 0;
	}

	for(i = 0; command[i] != '\0';i++)
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
		if(command[i] == '|')
		{
			if(command[i + 1] != '|')
			{
				if( i == 0 )
				{
					return 1;
				}
				if(i != 0 && command[i - 1] != '|')
				{
					return 1;
				}

			}
		}	
		if(command[i] == '>')
		{
			return 1;
		}

	}
	return 0;
}

int gotoredirect(char* command)
{
	pid_t pid = -1;
	if(0 == (pid = fork()))
	{
		execredirect(command);
		exit(0);	
	}
	else
	{
		waitpid(pid,NULL,0);
		usleep(1000*50);
		setmode();
		//printf("\n");
	}
	//write(STDOUT_FILENO,"redirect command\n",18);
	return 0;
}

int execredirect(char* command)
{
	int i = 0;
	int **pipe_array = NULL;
	pid_t *pid_array = NULL;
	int pipe_i = 0;
	int pipenumber = 0;
	int com_num = 0;
	int file_fd = -1;
	int count = 0;
	int mode = 0;
	char nextcom[MAXCOMMAND_SIZE];
	int havepipe = 1;
	int havefile = 1;
	int *state = 0;
	command_type temp_command;
	// get command number
	com_num = get_command_number(command);
	if(com_num <= 0)
	{
		printf("No command to redirect !!\n");
		return 0;
	}
	// get pipe number
	pipenumber = get_pipe_number(command);
	if(pipenumber<= 0)
	{
		havepipe = 0;
		//printf("No pipe !!\n");
//		return 0;
	}
	else
	{
		pid_array = (pid_t*)malloc(sizeof(pid_t) * com_num);
		state = (int*)malloc(sizeof(int) * com_num);
		pipe_array = malloc_pipearrry(pipenumber);
		if(NULL == pipe_array)
		{
			printf("pipe malloc error\n");
			return -1;
		}
		//pipe
		for(i = 0; i < pipenumber; i++)
		{
			if(0 > pipe(pipe_array[i]))
			{
				printf("creat pipe error\n");
			}
		}
	}
	//get file
	file_fd = get_file_fd(command,&mode);
	if(file_fd < 0)
	{
		havefile = 0;
	}
	// judge
	if(0 == havefile && 0 == havepipe)
	{
		return -1;
	}
	// get command
	if(-1 == get_next_command(command,nextcom,&count))
	{
		printf("Get next command error !!\n");
		return -1;
	}
	if(0 == havepipe)
	{
		redi_file(file_fd,nextcom,mode);
		close(file_fd);
		return 0;
	}
	//
	i = 0;
	pipe_i = 0;
	//printf("com_num : [%d]  pipe_number : [%d]\n",com_num,pipenumber);
	while(i < com_num)   // process rio
	{
		//printf("nextcom : \"%s\"\n",nextcom);
		if(0 > (pid_array[i] = fork()))
		{
			printf("fork error\n");
		}
		else if(0 == pid_array[i])  // son
		{
			if(0 == i)// first
			{
				if(0 > dup2(pipe_array[pipe_i][PIPEWRITE],STDOUT_FILENO))
				{
					printf("dup2 error \n");
				}
				close(pipe_array[pipe_i][PIPEREAD]);
			}
			else if(i == com_num - 1) //end
			{
				dup2(pipe_array[pipe_i - 1][PIPEREAD],STDIN_FILENO);
				if(-1 != file_fd)  // is have file
				{  
					if(1 == mode)
					{
						dup2(file_fd,STDOUT_FILENO);
					}
					if(2 == mode)
					{
						dup2(file_fd,STDERR_FILENO);
					}
					if(3 == mode)
					{
						dup2(file_fd,STDOUT_FILENO);
						dup2(file_fd,STDERR_FILENO);
					}
				}
				close(pipe_array[pipe_i - 1][PIPEWRITE]);
			}
			else
			{
				close(pipe_array[pipe_i][PIPEREAD]);
				dup2(pipe_array[pipe_i][PIPEWRITE],STDOUT_FILENO);
				close(pipe_array[pipe_i - 1][PIPEWRITE]);
				dup2(pipe_array[pipe_i - 1][PIPEREAD],STDIN_FILENO);
			}
			strcpy(temp_command.command_content,nextcom);
			//strcat(temp_command.command_content," &");
			temp_command.size = strlen(nextcom);
			close_all_switch();
			runcommand(&temp_command);
			//analysis_singlecommand(temp_command.command_content);
			if( i != com_num -1)
			{
				//char ch = EOF;
				//write(pipe_array[pipe_i][PIPEWRITE],&ch,1);
				close(pipe_array[pipe_i][PIPEWRITE]);
			}
			exit(0);
		}
		else  // fathergrg
		{
			//waitpid(pid_array[i],NULL,0);
			i++;
			pipe_i++;
			count++; // step '|'
			if(-1 == get_next_command(command,nextcom,&count))
			{
				printf("Get next command error !!\n");
				return -1;
			}
		}
	}
	//sleep(1);
	//for(i = 0 ; /*0 == all_stop(state,com_num)*/ i< com_num ; i++)
	//{
	//	waitpid(pid_array[i], state + i, WUNTRACED | WNOHANG);
		/*if(WIFSTOPPED(state[i]))
		{
			printf("pid : [%d]  stop\n",pid_array[i]);
		}*/
		/*if(WIFEXITED(state[i]) || WIFSIGNALED(state[i]))
		{
			printf("cloes pipe : [%d]\n", i);
			if(i != com_num - 1)
			{
				char writebuf[1] = {EOF};
				write(pipe_array[i][PIPEWRITE],writebuf,1);
				close(pipe_array[i][PIPEWRITE]);
				kill(pid_array[i],SIGCONT);
			}
			if(i != 0)
			{
				close(pipe_array[i - 1][PIPEREAD]);
			}
		}
		/*if(i + 1 == com_num)
		{
			i = -1;
		}*/
		//printf("pid : [%d] state : [%d]\n",pid_array[i],state[i]);
	//}

	if(file_fd > 0)
	{
		close(file_fd);
	}
	//free_pipearray(pipe_array,pipenumber);
	//free(pid_array);
	//free(state);
	return 0;
}

int all_stop(int* state_array,int num)
{
	int i = 0;
	if(NULL == state_array)
	{
		return -1;
	}
	for(i = 0 ; i < num; i++)
	{
		if(!WIFSTOPPED(state_array[i]))
		{
			return 0;
		}
	}
	return 1;
}

int redi_file(int file_fd, char* command,int mode)
{
	pid_t pid = 0;
	int state;
	command_type temp_command;
	if(file_fd < 0 || NULL == command)
	{
		return -1;
	}

	if(0 == (pid = fork()))
	{
		if(1 == mode)
		{
			dup2(file_fd,STDOUT_FILENO);
		}
		if(2 == mode)
		{
			dup2(file_fd,STDERR_FILENO);
		}
		if(3 == mode)
		{
			dup2(file_fd,STDOUT_FILENO);
			dup2(file_fd,STDERR_FILENO);
		}
		strcpy(temp_command.command_content,command);
		temp_command.size = strlen(command);
		runcommand(&temp_command);
		exit(0);
	}
	else
	{
		waitpid(pid, &state, 0);
	}

	return state;
}

int get_command_number(char* command)
{
	int i = 0;
	int num = 0;
	char ch;
	if(NULL == command || 0 == strlen(command))
	{
		return 0;
	}
	while('\0' != command[i])
	{
		if('\'' == command[i] || '\"' == command[i])
    	{
    		ch = command[i];
    		i++;
    		while(ch != command[i])
    		{
    			i++;
    		}
    	}
    	else
    	{
			if('|' == command[i])
			{
				num++;
			}
			i++;
		}
	}
	return num + 1;
}

int get_pipe_number(char* command)
{
	int i = 0;
	i = get_command_number(command);
	if(i == 0)
	{
		return 0;
	}
	return i - 1;
}

int get_file_fd(char* command,int* pmode)
{	
	int i = 0;
	int j = 0;
	int fd = 0;
	int filelocal = 0;
	int flag = O_RDWR | O_APPEND;
	mode_t op_mode =  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	char filename[DIRECTORY_SIZE];

	if(NULL == command || NULL == pmode)
	{
		return -1;
	}
	*pmode = 0;
	i = strlen(command) - 1;
	while( i >= 0 && command[i] != '>')
	{
		i--;
	}
	if( i < 0)
	{
		return -1;
	}
	// is or not creat
	filelocal = i + 1;
	if('>' == command[i - 1])
	{
		i--;
		flag = flag | O_CREAT;
	}
	// set pmode
	if('1' == command[i] || '2' == command[i])
	{
		*pmode = command[i];
	}
	else if('&' == command[i])
	{
		*pmode = 3;
	}
	else
	{
		*pmode = 1;
	}
	//get file name
	i = filelocal;
	while(' ' == command[i])
	{
		i++;
	}
	if('\0' == command[i])
	{
		printf("No files to be exported !!\n");
		return -1;
	}
	for(j = 0; command[i] != '\0';j++,i++)
	{
		filename[j] = command[i];
	}
	filename[j] = '\0';
	//  develop name
	if(0 < develop_abbdir(filename))
	{
		printf("Develop filename failed!!\n");
		return -1;
	}
	// open
	fd = open(filename,flag,op_mode);
	if(-1 == fd)
	{
		printf("Open file : \"%s\" error!!\n", filename);
	}
	//return 
	return fd;
}

int get_next_command(char* command ,char* nextcom,int* pcount)
{
	int i = 0;
	int j = 0;
	char ch;
	if(NULL == command || NULL == nextcom || NULL == pcount)
	{
		return -1;
	}

	i = *pcount;
	while(command[i] != '\0' && command[i] != '|' && command[i] != '>')
	{
		if('\'' == command[i] || '\"' == command[i])
    	{
    		ch = command[i];
    		nextcom[j] = command[i];
    		i++;
    		j++;
    		while(ch != command[i])
    		{
    			nextcom[j] = command[i];
    			i++;
    			j++;
    		}
    		nextcom[j] = command[i];
    		i++;
    		j++;
    	}
    	else
    	{
			nextcom[j] = command[i];
			i++;
			j++;
		}
		/* code */
	}
	*pcount = i;
	nextcom[j] = '\0';
	clear_superfluous_space(nextcom);

	return 0;
}

int** malloc_pipearrry(int num)
{
	int **retp = NULL;
	int i = 0;
	retp = (int**)malloc(sizeof(int*) * num);
	for(i = 0; i < num ;i++)
	{
		retp[i] = (int*)malloc(sizeof(int) * 2);
	}
	return retp;
}

void free_pipearray(int** pipe_array,int num)
{
	int i = 0;
	if(NULL == pipe_array)
	{
		return ;
	}
	for(i = 0 ; i < num ; i++)
	{
		free(pipe_array[i]);
	}
	free(pipe_array);
}