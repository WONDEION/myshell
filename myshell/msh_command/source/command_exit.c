#include "command_exit.h"

int command_exit(int argc, char* arg[])
{
	if(0 == argc || NULL == arg)
	{
		return -1;
	}
	if(strcmp("exit",arg[0]))
	{
		write(STDERR_FILENO,"No call shell command ",22);
		write(STDERR_FILENO,"exit",strlen("exit"));
		write(STDERR_FILENO," !! \n",5);
	}
	if(0 != quitclear())
	{
		perror("myShell quit and clear error!!!\n\r");
	}
	exit(0);
	return 0;
}