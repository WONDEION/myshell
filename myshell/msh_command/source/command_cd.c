#include "command_cd.h"

int command_cd(int argc,char* arg[])
{
	int state = 0;
	int i = 0;
	int *mark = NULL;
	char dir[DIRECTORY_SIZE];
	if(NULL == arg || 0 == argc)
	{
		return -1;
	}
	if(strcmp(arg[0],"cd"))
	{
		shellcommandNOcall(printf_fd,"cd");
		return -1;
	}
	mark = (int*)malloc(argc * sizeof(int));
	clear_intarray(mark,argc);
	mark[0] = 1;
	
	//print_arg(argc,arg);
	if(0 < (i =find_char_from_arg(argc,arg,'L')))
	{
		state = cd_option(argc,arg,'L',dir);
		mark[i] = 1;
	}
	else if(0 < (i =find_char_from_arg(argc,arg,'P')))
	{
		state = cd_option(argc,arg,'P',dir);
		mark[i] = 1;
	}
	else if(0 < (i =findchar_from_arg(argc,arg,'-')))
	{
		get_from_sys_env("OLDPWD",dir);
		mark[i] = 1;
	}
	else if(arg[1] == NULL)
	{
		printf("home : %s\n", home.directory);
		strcpy(dir,home.directory);
	}
	else
	{
		if(-1 == (i = get_chdir(arg,dir)))
		{
			return -1;
		}
		mark[i] = 1;
	}
	state = chdir(dir);
	if(state == -1)
	{
		printf("chdir error!!\n");
		return -1;
	}
	// set env
	updatesysenv("PWD",dir);
	updatesysenv("OLDPWD",nowdir);
	// set nowdir
	set_nowdir(dir);
	if(Nooptions(printf_fd, "cd",mark,arg))
	{
		state = 1;
	}
	free(mark);
	return state;
}

int cd_option(int argc,char* arg[],char ch,char* dir)
{
	return 0;
}

int get_chdir(char* arg[],char* dir)
{
	int i = 0;
	if(NULL == arg || NULL == dir)
	{
		return -1;
	}
	for( i = 1; arg[i] != NULL; i++)
	{
		if(arg[i][0] != '-')
		{
			break;
		}
	}
	if(NULL == arg[i])
	{
		printf("No directory parameters!!\n");
		return -1;
	}
	strcpy(dir,arg[i]);
	if(-1 == develop_abbdir(dir))
	{
		printf("Directory parameters is invalid!!\n");
		return -1;
	}

	if(!testdir(dir))
	{
		return -1;
	}

	return i;
}
int set_nowdir(char* dir)
{
	int i = 0;
	if(NULL == dir)
	{
		return -1;
	}
	for(i = 0 ; dir[i] != '\0'; i++)
	{
		nowdir[i] = dir[i];
		workdir.directory[i] = dir[i];
	}
	nowdir[i] = '\0';
	workdir.directory[i] = '\0';
	return 0;
}

int develop_abbdir(char* dir)
{
	char buf[DIRECTORY_SIZE];
	if(NULL == dir)
	{
		return -1;
	}
	//printf("dir : \"%s\"\n", dir);
	if('.' == dir[0] && '.' != dir[1] )  //.~~
	{
		if('/' == dir[1])  // ./
		{

			if(!islegaldirectory(dir + 2))
			{
	
				return -1;
			}
			strcpy(buf,nowdir);
			strcat(buf,dir + 1); // include '/'
			strcpy(dir,buf);
		}
		else if('\0' == dir[1])  // .
		{
			strcpy(dir,nowdir);
		}
		else  // .string
		{
			if(!islegaldirectory(dir + 1))
			{
				return -1;
			}
			strcpy(buf,nowdir);
			if(strcmp(nowdir,"/"))
			{
				strcat(buf,"/");
			}
			strcat(buf,dir);
			strcpy(dir,buf);
		}
		return 0;
	}
	else if('.' == dir[0] && '.' == dir[1] )  // ..~~
	{
		if('/' == dir[2])  // ../
		{
			if(!islegaldirectory(dir + 3))
			{
				return -1;
			}
			strcpy(buf,nowdir);
			delete_lastlv_dir_string(buf);
			strcat(buf,dir +2);  // include '/'
			strcpy(dir,buf);
		}
		else if('\0' == dir[2])  // ../
		{
			strcpy(buf,nowdir);
			delete_lastlv_dir_string(buf);
			strcpy(dir,buf);
		}
		else   // ..string
		{
			if(!islegaldirectory(dir + 1))
			{
				return -1;
			}
			strcpy(buf,nowdir);
			if(strcmp(nowdir,"/"))
			{
				strcat(buf,"/");
			}
			strcat(buf,dir);
			strcpy(dir,buf);
		}
		return 0;
	}
	else if('/' == dir[0])
	{
		return 0;
	}
	else
	{
		if(!islegaldirectory(dir))
		{
			return -1;
		}
		strcpy(buf,nowdir);
		if(strcmp(nowdir,"/"))
		{
			strcat(buf,"/");
		}
		strcat(buf,dir);
		strcpy(dir,buf);
		return 0;
	}
	return 0;
}

int testdir(char* dir)
{
	struct stat rest;
	int i = 0;
	if(NULL == dir)
	{
		return 0;
	}
	//printf("in test dir : %s\n", dir);
	if(-1 == stat(dir,&rest))
	{
		Nodir_or_file(STDOUT_FILENO,dir);
		return 0;
	}
	if(S_ISDIR(rest.st_mode))
	{
		return 1;
	}
	//printf("Parameters is not directory!!\n");
	return 0;

}

int isdirchar(char ch)
{
	if(isalnum(ch))
	{
		return 1;
	}
	if('_' == ch)
	{
		return 1;
	}
	if( '?' == ch || ',' == ch || '@' == ch || '#' == ch || '$' == ch 
		|| '&' == ch || '(' == ch || ')' == ch || '\'' == ch || ';' == ch 
		|| '\"' == ch || '<' == ch || '>' == ch || '.' == ch || '-' == ch)
	{
		return 1;
	}
	return 0;
}

int islegaldirectory(char* dir)
{
	int i =0;
	if(NULL == dir)
	{
		return -1;
	}
	for( i = 0 ; dir[i] != '\0' ;i++)
	{
		if(!isdirchar(dir[i]))
		{
			return 0;
		}
	}
	return 1;
}