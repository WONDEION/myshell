#include "readline.h"

extern char disline_choice ;

// return -2 no run
int readline(int fd, command_type* pcommand)      //  
{
	command_type temp_command;
	int command_size = 0;
	int st = 0;
	//
	if(clearcommand(pcommand))   // begin new command input
	{
		PRINTERROR("readline - clearcommand \r\n");
	}
	clear_commandstack(&stack_command);
	nowcommand_point = &temp_command;
	while(1)
	{
		if(1 == display_open)
		{
			display_linehead();  // print head of line 
		}
		if(clearcommand(&temp_command))   // begin new command input
		{
			PRINTERROR("readline - clearcommand \r\n");
		}
		clear_commandstack(&stack_command);
		if(0 <= (st = getline_m(fd,&temp_command)))    //return size of command
		{
			// Existence input
			if(1 == display_open)
			{
				write(STDOUT_FILENO,"\r\n",2);  // print 13
			}
			//if(0 != temp_command.size)
			//{
			
			if(combinecommand(pcommand,&temp_command))
			{
				PRINTERROR("readline - combinecommand \r\n");
			}
			if(isfinished(pcommand))   // if the command have finished  
			{
				break;
			}
		//	}
		//	else  //size == 0;
		//	{
		//		return 0;
		//	}
		}
		else  //no run this command
		{
			if(1 == display_open)
			{
				write(STDOUT_FILENO,"\r\n",2);  // print 13
			}
			pcommand->command_content[pcommand->size] = '\0';
			clear_commandstack(&stack_command);
			return st;
		}
	}
	nowcommand_point = NULL;
	clear_commandstack(&stack_command);
	return pcommand->size;
}
// return -2 no run
int getline_m(int fd, command_type* pcommand)    //return how mant input to command
{
	char inputchar = 0;
	int size = 0;        // how many input
	char shift = 0;
	int functionnum = 0;
	int st = 0;

	if(NULL==pcommand)
	{
		PRINTERROR("getline pcommand is NULL\r\n");
	}
	while(1)
	{	
		inputchar = msh_getchar(fd);    //get char
		due_char = 1;
		//printf("inputchar : %d\n",inputchar);
		if(-1 == inputchar)
		{
			//printf("here\n");
			if(0 == size)
			{
				//printf("here\n");
				due_char = 0;
				return -1;
			}
			//return -1;
			due_char = 0;
			break;
		}
		//printf("get : %d\n",inputchar);
		size++;
		if(1 == screenlock)   // lock screen
		{
			if(17 == inputchar)
			{
				charag_functiontable[17](pcommand);   
			}
			else
			{
				addchar_lockcache(inputchar);
			}
		}
		else   // unlock
		{
			if(13 == inputchar || 0 == inputchar || 10 == inputchar)   // in org mode '\n' != enter
			{
				due_char = 0;
				break;
			}
			if(shift != 0)    // if last times is 27 or 91 and 5~
			{
				functionnum = shiftjudeg(shift,inputchar);
				//printf("\r\ninputchar : %d    functionnum : %d\r\n",inputchar,functionnum);
				if(-1 == functionnum)
				{
					// match failed
					// Discarding this character
					shift = 0; //clear 
					due_char = 0;
					continue;  
				}
				else if(0 == functionnum)
				{ 
					// updata shift
					shift = inputchar;  
				}
				else
				{
					shift = 0;
					if(functionnum > 50)
					{
						PRINTERROR("At getline : functionnum>=50\r\n");	
					}
					else
					{
						// save state
						push_command(&stack_command,pcommand);
						//  Call control function
						if(0 > (st = charag_functiontable[functionnum](pcommand)))  // no run
						{
							if( -2 == st )  // over input and run
							{
								due_char = 0;
								return size;
							}
							else
							{
								due_char = 0;
								return st;
							}
						}		
					}			
				}
			}
			else    // normal input 
			{
				if(inputchar >= 32 && inputchar != 127 )   // displayable code
				{
					add_and_print(pcommand,inputchar);		
				}
				else   // control code
				{
					if(27 == inputchar)    // this input is a star of compound key 
					{
						shift = 27;
					}
					else     // Direct control keys
					{
						// save state
						push_command(&stack_command,pcommand);
						// call function
						//printf("\r\n get : %d\r\n",inputchar);
						if( 127 == inputchar )
						{
							charag_ctrlH(pcommand);
						}
						else if(0 > (st = charag_functiontable[inputchar](pcommand)))  // no run
						{
							if( -2 == st )  // over input and run
							{
								due_char = 0;
								return size;
							}
							else
							{
								due_char = 0;
								return st;
							}
						}	
					}
				}	
			}
		}
		due_char = 0;
	}
	//printf("here\n");
	//adddiscode_to_command(pcommand,'\0');   // Function addcode is add at cursor position 
	pcommand->command_content[pcommand->size] = '\0';
	due_char = 0;
	return size;
}

int add_and_print(command_type* pcommand,char inputchar)
{
	// append in command
	addiscode_to_command(pcommand,inputchar);
	if(1 == display_open)
	{
		printchar(pcommand,inputchar);   // print to screen 
	}
	return 0;
}


int isfinished(command_type* pcommand)   // if doesnot completed  add ' ' at tail   0 - normal
{ 
	int quotes_times = 0;
	int double_quotes_times = 0;
	int have_if = 0;
	int have_done = 0;
	int have_case = 0;
	char buf[MAXCOMMAND_SIZE];
	int i = 0;
	//Sentence judgment
	if(ischangeline(pcommand))
	{
		disline_choice = 1;
		return 0;
	}
	have_if = get_word_times(pcommand->command_content,"if");
	have_done += get_word_times(pcommand->command_content,"for");
	have_done += get_word_times(pcommand->command_content,"while");
	have_done += get_word_times(pcommand->command_content,"until");
	have_done += get_word_times(pcommand->command_content,"select");
	have_case = get_word_times(pcommand->command_content,"case");
	// Sentence judgment
	have_if -= get_word_times(pcommand->command_content,"fi");
	have_done -= get_word_times(pcommand->command_content,"done");
	have_case -= get_word_times(pcommand->command_content,"esac");
	if( 0 == have_if && 0 == have_case && 0 == have_done)
	{
		disline_choice = 0;
		return 1;
	}
	else
	{
		disline_choice = 1;
		return 0;
	}
	
	disline_choice = 0;
	return 1;
} 

int ischangeline(command_type* pcommand)
{
	int i = 0;
	if(NULL == pcommand)
	{
		return 0;
	}
	if(0 >= pcommand->size)
	{
		return 0;
	}
	i = strlen(pcommand->command_content) - 1;
	while(i >=0 && ' ' == pcommand->command_content[i])
	{
		i--;
	}
	if('\\' == pcommand->command_content[i])
	{
		pcommand->command_content[i] = '\0';
		pcommand->size = i;
		pcommand->cursorlocal = i;
		return 1;
	}
	return 0;
}	

int printcommandinfo(command_type* pcommand)
{
	int i = 0;
	printf("\r\n\
	command_content[MAXCOMMAND_SIZE]: %s\r\n\
	size: %d \r\n\
	line: %d \r\n\
	screenline: %d \r\n\
	cursorlocal: %d	\r\n\
	line_position.x: %d \r\n\
	line_position.y: %d \r\n\
	cursor_position.x: %d \r\n\
	cursor_position.y: %d \r\n\
 	pushstack_times: %d \r\n ",
	pcommand->command_content,
	pcommand->size,
	pcommand->line,
	pcommand->screenline,
	pcommand->cursorlocal,
	pcommand->line_position.x,
	pcommand->line_position.y,
	pcommand->cursor_position.x,
	pcommand->cursor_position.y,
	pcommand->pushstack_times);
	for(i = 0; i < pcommand->line; i++)
	{
		printf("	linesize[%d]: %d\r\n",i,pcommand->linesize[i]);
	}
	for(i = 0; i < pcommand->screenline; i++)
	{
		printf("	slinesize[%d]: %d\r\n",i,pcommand->slinesize[i]);
	}
}

int print_nowscreen()
{
	display_linehead();
	if(NULL == nowcommand_point)
	{
		return -1;
	}
	//printcommandinfo(nowcommand_point);
	//printf("%s", nowcommand_point->command_content);
	write(STDOUT_FILENO,nowcommand_point->command_content,nowcommand_point->size);
	updatecursor(nowcommand_point, nowcommand_point->cursorlocal);
	return 0;
}
