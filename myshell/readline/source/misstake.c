#include "misstake.h"

int shellcommandNOcall(int fd,char* name)
{
	if(fd < 0 || NULL == name)
	{
		return -1;
	}
	write(fd,"No call shell command ",22);
	write(fd,name,strlen(name));
	write(fd," !! \n",5);

	return 0;
}

int Nodir_or_file(int fd,char* dir)
{
	if(fd < 0 || NULL == dir)
	{
		return -1;
	}
	write(fd,"No such file or directory : ",28);
	write(fd,dir,strlen(dir));
	write(fd," !\n",3);

	return 0;
}

int Nooptions(int fd, char* commandname, int mark[], char** arg)
{
	int i = 0;
	int times = 0;
	int retstate = 0;
	if(NULL == commandname || NULL == mark || NULL == arg || fd < 0)
	{
		return -1;
	}
	for(i = 0; arg[i] != NULL ;i++)
	{
		if(0 == mark[i])
		{
			retstate++;
			if(0 == times)
			{
				times = 1;
				write(fd,"command \"",9);
				write(fd,commandname,strlen(commandname));
				write(fd,"\"cann\'t to handle parameter :",29);
			}
			write(fd,"[",1);
			write(fd,arg[i],strlen(arg[i]));
			write(fd,"] ",2);
		}
	}
	if(0 != times)
	{
		write(fd,"\n",1);
	}
	return retstate;
}