#include "command_alias.h"

envtable alias_table;
char aliastable_dir[DIRECTORY_SIZE];
char* aliastable_name = ".msh_aliasfile";
int aliasfile_fd = -1;
int ifchange_table = 0;  // 1 : wo need rewrite
char aliasfile_head[] = "This is myshell alias file\n";

int inti_alias()
{
	get_aliasdir();
	alias_table.size = 0;
	alias_table.number = 0;
	alias_table.ptable = NULL;
	//printf("%s\n",aliastable_dir);
	aliasfile_fd = open_and_creat_alias();
	if(aliasfile_fd > 0)
	{
		printf("Open aliasfile success!!\n");
	}
	//printf("herhe\n");
	//printenvtable(&alias_table);
	read_to_aliastable(aliasfile_fd,&alias_table);
	close(aliasfile_fd);
	aliasfile_fd = -1;
	ifchange_table = 0;
	//write(STDOUT_FILENO,"All alias :\n",12);
	//printenvtable(&alias_table);
	return 0;
}	

int get_aliasdir()
{
	strcpy(aliastable_dir,user_inf->pw_dir);
	strcat(aliastable_dir,"/");
	strcat(aliastable_dir,aliastable_name);
	return 0;
}

int open_and_creat_alias()
{
	if(-1 == access(aliastable_dir,F_OK))
	{
		aliasfile_fd = open(aliastable_dir ,(O_CREAT|O_RDWR));
		if(-1 == aliasfile_fd)
		{
			printf("Creat aliasfile error\n");
		}
		if(-1 == chmod(aliastable_dir,(S_IRUSR|S_IWUSR|S_IROTH|S_IRGRP)))
		{
			printf("chmod aliasfile error\n");
		}	
		//printf("aliasfile_fd : %d\n",aliasfile_fd);
		write_essential_information();
	}
	else  // 
	{
		aliasfile_fd = open(aliastable_dir,O_RDWR);
	}
	return aliasfile_fd;
}

int write_essential_information()
{
	environment_variable temp_env;
	//printf("aliasfile_fd : %d\n",aliasfile_fd);
	if( -1 == write(aliasfile_fd, aliasfile_head, strlen(aliasfile_head)))
	{
		printf("write error : %d\n",errno);
	}
	// ls
	write(aliasfile_fd, "ls\nls --color=auto\n",19);
	//addto_envtable(aliastbale, &temp_env);
	// grep
	write(aliasfile_fd, "grep\ngrep --color=auto\n",23);
	//addto_envtable(aliastbale, &temp_env);
	return 0;
}

int command_alias(int argc , char* arg[])
{
	environment_variable temp_env;
	int i = 0;
	int *mark = NULL;
	int retstate = 0;
	char temp_string[MAXCOMMAND_SIZE];
	if(0 == argc || NULL == arg)
	{
		return -1;
	}
	if(strcmp(arg[0],"alias"))
	{
		shellcommandNOcall(STDERR_FILENO,"alias");
		return -1;
	}
	mark = malloc(sizeof(int) * argc);
	clear_intarray(mark, argc);
	mark[0] = 1;
	if(-1 != (i = find_char_from_arg(argc, arg, 'p')))
	{
		mark[i] = 1;
		write(STDOUT_FILENO,"All alias :\n",12);
		printenvtable(&alias_table);
	}
	// get string
	temp_string[0] = '\0';
	for(i = 1; NULL != arg[i]; i++)
	{
		if(!strcmp("-p",arg[i]))
		{
			continue;
		}
		mark[i] = 1;
		if('\0' != temp_string[0])
		{
			strcat(temp_string," ");
		}
		strcat(temp_string,arg[i]);
	}
	//bulid
	if(-1 == ass_alias_string(&temp_env,temp_string))
	{
		retstate = 2;
	}
	else  // ass success
	{
		ifchange_table++;
		addto_envtable(&alias_table, &temp_env);
	}	
	if(Nooptions(STDERR_FILENO, "alias", mark, arg))
	{
		retstate = 1;
	}
	free(mark);
	return retstate;
}

int ass_alias_string(environment_variable* penv, char* string)
{
	int i = 0;
	int j = 0;
	if(NULL == penv || NULL == string || '\0' == string[0])
	{
		return -1;
	}
	// get name
	i = 0;
	j = 0;
	while(string[i] != '=' && string[i] != '\0')
	{
		penv->name[j] = string[i];
		i++;
		j++;
	}
	if('\0' == string[i])
	{
		printf("alias build format error!!\n");
		return -1;
	}
	penv->name[j] = '\0';
	i++;  //step '='
	// get value
	j = 0;
	while(string[i] != '\0')
	{
		penv->value[j] = string[i];
		i++;
		j++;
	}
	penv->value[j] = '\0';
	return 0;
}

int develop_alias(char* command)
{
	char command_name[MAXCOMMAND_SIZE];
	char replace[MAXCOMMAND_SIZE];
	int i = 0;
	int j = 0;
	int local = 0;
	int nameleng = 0;
	int replaceleng = 0;

	if(NULL == command)
	{
		return -1;
	}
	while(-1 != gotonext_com(command, &local))
	{
		i = local;
		get_first_word(command + i, command_name);
		local = local + strlen(command_name);
		//printf("command_name : %s",command_name);
		if(-1 != get_from_envtable(&alias_table,command_name,replace))
		{
			nameleng = strlen(command_name);
			replaceleng = strlen(replace);
			j = 0;
			string_translation(command,i,replaceleng - nameleng);    // 0< left   0> right
			//replace
			for(j = 0; replace[j] != '\0' ; j++)
			{
				command[i + j] = replace[j];
			}
			// adjust i for command
			i = i + j;
			local = i;
		}

	}

	return 0;
}

int gotonext_com(char* command, int* local) // return means end  
// get next spacer 
{
	int i = 0;
	int ret = 0;
	int step = 0;
	
	if(NULL == command)
	{
		return -1;
	}
	if(0 == *local)
	{
		return 0;
	}

	i = *local;
	while(0 == (step = is_commandspacer(command + i)))
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
		}
		i++;
	}
	if(-1 == step)
	{
		return -1;
	}
	i = i + step;
	while(' ' == command[i])
	{
		i++;
		step++;
	}
	*local = i;
	return step;
}

int read_to_aliastable(int aliasfile_fd, envtable* palias_table)
{
	environment_variable temp_env;
	int i = 0;
 	char buf_c = 0;
 	char name[MAXCOMMAND_SIZE];
 	char value[MAXCOMMAND_SIZE];
 	int who = 0; // 0== name  1 == value
 	int change = 0;

	if(-1 == aliasfile_fd || NULL == palias_table)
	{
		return -1;
	}

	lseek(aliasfile_fd, strlen(aliasfile_head), SEEK_SET);
	while(0 != read(aliasfile_fd,&buf_c,1))
	{
		if('\n' == buf_c)
		{
			change++;
			if(2 == change)  // if read all end
			{
				value[i] = '\0';
				ass_env(&temp_env,name,value);
				//printf("addtimes\n");
				addto_envtable(palias_table, &temp_env);  // this
				i = 0;
				change = 0;
				who = 0;
				continue;
			}
			else if( 1 == change)  // if read name end
			{
				name[i] = '\0';
				i = 0;
				who = 1;
				continue;
			}
		}
		if(0 == who) 
		{
			name[i] = buf_c;
			i++;
		}
		else if(1 == who)
		{
			value[i] = buf_c;
			i++;
		}
	}
	return 0;
}

int close_alias()
{
	if(0 != ifchange_table)
	{
		write_back_aliasfile();
	}
	clear_env(&alias_table);
	return 0;
}

int write_back_aliasfile()
{
	int i = 0;
	if(-1 == (aliasfile_fd = open(aliastable_dir , (O_CREAT | O_RDWR | O_TRUNC))))
	{
		printf("write_back_aliasfile : open alias file error !!\n");
		return -1;
	}
	write(aliasfile_fd,aliasfile_head,strlen(aliasfile_head));
	for(i = 0; i < alias_table.number; i++)
	{
		write(aliasfile_fd, alias_table.ptable[i].name
			,strlen(alias_table.ptable[i].name));
		write(aliasfile_fd, "\n", 1);
		write(aliasfile_fd, alias_table.ptable[i].value
			,strlen(alias_table.ptable[i].value));
		write(aliasfile_fd, "\n", 1);
	}
	return 0;
}

int isalias(char* string)
{
	int i = 0;
	if(NULL == string)
	{
		return 0;
	}
	for(i = 0; i < alias_table.number; i++)
	{
		if(!strcmp(string, alias_table.ptable[i].name))
		{
			return 1;
		}
	}
	return 0;
}