#include "linuxcommand.h"

char** command_path = NULL;
int command_path_size = 0;

int get_path()
{
	int i = 0;
	int j = 0;
	int p_i = 0;
	int n = 0;
	char value[MAXENV_SIZE];
	if(!get_from_sys_env("PATH", value))
	{
		printf("get_from_sys_env error\n");
		return -1;
	}
	//printf("get_from_sys_env success!!\n");
	//printf("%s\n",value);
	for( i = 0, n = 0; value[i] != '\0';i++)
	{
		if(value[i] == ':')
		{
			n++;
		}
	}
	n++;
	if(command_path == NULL || n > command_path_size)
	{
		if(-1 == malloc_command_path(n))
		{
			return -1;
		}
		//printf("malloc_command_path success !!\n");
	}

	for(i = 0, j =0, p_i =0; value[i] != '\0'; i++ ,j++)
	{
		//printf("in loop\n");
		if(':' == value[i])
		{
			command_path[p_i][j] = '\0';
			j = -1;
			p_i++;
		}
		else
		{
			command_path[p_i][j] = value[i];
		}
	}
	command_path[p_i][j] = '\0';
	command_path[p_i + 1] = NULL;
	//printf("get_path success !!\n");
	/*for(i = 0 ;i< command_path_size;i++)
	{
		printf("%s\n", command_path[i]);
	}*/
	return 0;
}

int malloc_command_path(int n)
{
	int i = 0;
	if(command_path == NULL)
	{
		command_path = (char**)malloc((n + 1) * sizeof(char*));
		if(command_path == NULL)
		{
			write(STDERR_FILENO,"malloc error\n",13);
			return -1;
		}
		for(i = 0; i < n + 1; i++)
		{
			command_path[i] = (char*)malloc(DIRECTORY_SIZE * sizeof(char*));
			if(NULL == command_path[i])
			{
				write(STDERR_FILENO,"malloc error\n",13);
				return -1;
			}
		}
		command_path_size = n + 1;

	}
	else
	{
		if(n > command_path_size)
		{
			command_path = (char**)realloc(command_path,n + 1* sizeof(char*));
			if(command_path == NULL)
			{
				write(STDERR_FILENO,"malloc error\n",13);
				return -1;
			}
			for(i = command_path_size; i < n + 1; i++)
			{
				command_path[i] = (char*)malloc(DIRECTORY_SIZE * sizeof(char*));
				if(NULL == command_path[i])
				{
					write(STDERR_FILENO,"malloc error\n",13);
					return -1;
				}
			}
			command_path_size = n + 1;
		}
	}
	return 0;
}

int free_command_path()
{
	int i = 0;
	for(i = 0; i < command_path_size;i++)
	{
		free(command_path[i]);
	}
	free(command_path);
}

int test_linuxcommand(char* name, char* directory) // find at directory
// error :-1  Non-existent :0 success : 1
{
	DIR* dp;
	struct dirent* dirp;
	if(NULL == name || NULL == directory)
	{
		return -1;
	}
	
	dp = opendir(directory);
	if(NULL == dp)
	{
		//printf("directory : \"%s\" is existence!!\n",directory);
		return -1;
	}

	while((dirp = readdir(dp)) != NULL)
	{
		if(0 == strcmp(name,dirp->d_name))
		{
			char tempdir[DIRECTORY_SIZE];
			strcpy(tempdir,directory);
			strcat(tempdir,"/");
			strcat(tempdir,name);
			if(0 == access(tempdir,X_OK))
			{
				return 1;
			}
			else
			{
				//printf("%s\n",tempdir);
				//printf("File type error!!\n");
				return 0;
			}
		}
	}

	return 0;
}
 
int get_linuxcommand_dir(char* name, char* directory)
// error :-1  Non-existent :0 success : 1
{
	int i = 0;
	char temp[DIRECTORY_SIZE]; 

	if(NULL == name || NULL == directory)
	{
		return -1;
	}

	if(-1 == get_path())
	{
		return -1;
	}
	//printf("get_path success !!\n");
	while(command_path[i] != NULL)
	{
		if(1 == test_linuxcommand(name,command_path[i]))
		{
			strcpy(temp,command_path[i]);
			strcat(temp,"/");
			strcat(temp,name);
			strcpy(directory,temp);
			return 1;
		}
		i++;
	}
	return 0;
}

int execute_linuxcommand(char* command)
{
	char comname[MAXCOMMAND_SIZE];
	char dir[DIRECTORY_SIZE];
	int argc = 0;
	int i = 0;
	char **arg;
	int nowait = 0; 
	pid_t pid;
	pid_t s_pid;
	int state;
	//printf("exec times\n");
	if(NULL == command)
	{
		return -1;
	}
	get_first_word(command,comname);
	//printf("get first word success  !!\n");
	if(1 != get_linuxcommand_dir(comname,dir))
	{
		printf("No linux command : \"%s\"!!!\n",comname);
		return 0;
	}
	//printf("get_linuxcommand_dir sucess!!\n");
	sonwait = 1;
	if(-1 != find_word_from_string(command,"&"))
	{
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
	//printf("malloc_arg success!!\n");
	// 
	string_to_arg(command,arg);
	//printf("pcommand : %s\n",pcommand->command_content);
	/*for(i = 0 ; arg[i]!= NULL;i++)
	{
		printf("arg[i] : %s\n", arg[i]);
	}*/
	// fork
	//printf("%s\n",dir);
	state = fork_exec(dir,command,argc,arg);
	return state;
}

int islinuxcommand(char* command)
{
	int i = 0;
	char name[NAME_MAX];
	if(NULL == command)
	{
		return 0;
	}
	get_path();
	get_first_word(command,name);
	while(command_path[i] != NULL)
	{
		if(1 == test_linuxcommand(name,command_path[i]))
		{
			return 1;
		}
		i++;
	}
	return 0;
}

