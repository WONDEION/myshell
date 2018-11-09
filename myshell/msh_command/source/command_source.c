#include "command_source.h"

int command_source(int argc, char* arg[])
{
	char filename[DIRECTORY_SIZE];
	int fd = -1;
	command_type command;
	command_type temp_command;
	if(0 == argc || NULL == arg)
	{
		return -1;
	}
	if(strcmp(arg[0],"source"))
	{
		shellcommandNOcall(STDOUT_FILENO,"source");
		return -1;
	}
	if(NULL == arg[1])
	{
		printf("No parameter input!!\n");
		return -1;
	}
	strcpy(filename,arg[1]);
	if(-1 == develop_abbdir(filename))
	{
		printf("invalid file name !!\n");
		return -1;
	}
	//test
	if(-1 == test_file(filename))
	{
		return -1;
	}
	// open
	fd = open(filename,O_RDONLY,0);
	if(-1 == fd)
	{
		printf("Open file : \"%s\" error!!\n", filename);
		return -1;
	}
	// read and run
	clearcommand(&temp_command);
	clearcommand(&command);
	display_open = 0;
	//while(-1 != readline_fromfile(fd,&command))	
	while(-1 != readline(fd,&command))
	{
		//printf("here\n");
		runcommand(&command);
		//printf("here\n");
	}
	display_open = 1;
	if(argc > 2)
	{
		return 1;
	}
	return 0;
}

int test_file(char* filename)
{
	struct stat rest;// = NULL;
	if(NULL == filename)
	{
		return -1;
	}
	if(-1 == stat(filename,&rest))
	{
		printf("File : \"%s\" is nonexistent!!\n",filename);
		return -1;
	}
	if(-1 == access(filename,R_OK))
	{
		printf("Jurisdiction error!!\n");
		return -1;
	}
	if(!S_ISREG(rest.st_mode))
	{
		printf("File type error!!\n");
		return -1;
	}
	return 0;
}

