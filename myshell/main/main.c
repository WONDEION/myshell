#include "main.h"
//#include "./readline/head/readline.h"
//#include "./readline/head/Datastruct.h"


int main(int argc, char* argv[])
{
	command_type command;
	char inputc = 0;
	int state;
	int i = 0;
	int lastcursor = 0;

	clearcommand(&command);

	printhello();

	if(0!=initialize())
	{
		perror("myShell initialize error!!!\n\r");
	}
	/*print_arg(argc,argv);
	i = find_string_from_arg(argc, argv,"go");
	printf("i : %d\n",i);*/
	if(-1 != (i = find_string_from_arg(argc, argv,"go")))
	{
		arg_to_string(argv + i + 1, command.command_content);
		state = runcommand(&command);
		quitclear();
		return state;
	}
	printf("initialized finished !\r\n");

	while(1)
	{
		clearcommand(&command);
		on_read = 1;
		state = readline(STDIN_FILENO,&command);     // get command
		on_read = 0;
		push_command(&stack_command,&command);
		updatecursor(&command,command.size);
		pop_command(&stack_command,&command);
		//printf("\r\n out read state: %d \r\n",state);
		//write(STDOUT_FILENO,"\r\n",2);  // print 13
		add_and_clearchoice(&command);

		//write(STDOUT_FILENO,"\r\nout read\r\n",12);
		if(state > 0)
		{
			runcommand(&command);    // run
		}
		else   // no input or no run
		{
			//printcommandinfo(&command);
		}
	}
	//print_histable(0,history_table.size - 1);

	return 0;
}

int runcommand(command_type* pcommand)  //  return -1 when the command is 'exit'  
{
	//printcommand(pcommand);
	int i = 0;
	while(' ' == pcommand->command_content[i] || '\t' == pcommand->command_content[i])
	{
		i++;
	}
	if('#' == pcommand->command_content[0])  // Notes
	{
		return 0;
	}
	if( -1 == process_input(pcommand->command_content))
	{
		return 0;
	}
	if(0 == pcommand->size || '\0' == pcommand->command_content[0])
	{
		return 0;
	}
	//printf("here2\n");
	//printf("run times\n");
	analysis_command_primary(pcommand->command_content);
	//printcommandinfo(pcommand);
	return 0;
}

int printcommand(command_type* pcommand)
{
	//printf("inrun\r\n");
	//printf("%d   %s\r\n",strlen(pcommand->command_content),pcommand->command_content);
	write(STDOUT_FILENO,pcommand->command_content,strlen(pcommand->command_content));
	write(STDOUT_FILENO,"\r\n",2);
	return 0;
}



int printhello()
{
	printf("hello !\r\n");
}