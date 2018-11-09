
#include "charanalysis.h"


int screenlock = 0;   // 1 lock   0 unlock

charag_functiontable_type charag_functiontable[51] = {
													NULL, 
										/*1*/		charag_ctrlA, charag_ctrlB, charag_ctrlC, charag_ctrlD, charag_ctrlE,
										/*6*/		charag_ctrlF, charag_ctrlG, charag_ctrlH, charag_ctrlI, charag_ctrlJ,
										/*11*/		charag_ctrlK, charag_ctrlL, charag_ctrlM, charag_ctrlN, charag_ctrlO,
										/*16*/		charag_ctrlP, charag_ctrlQ, charag_ctrlR, charag_ctrlS, charag_ctrlT,
										/*21*/		charag_ctrlU, charag_ctrlV, charag_ctrlW, charag_ctrlX, charag_ctrlY,
/*0*/
										/*26*/		charag_ctrlZ, charag_ctrl27, charag_ctrl28, charag_ctrl29, charag_ctrl30,
										/*31*/		charag_ctrl31, charag_ctrl32, charag_altB, charag_altF, charag_altR, 
										/*36*/		charag_altL, charag_altU, charag_altP, charag_acE, charag_alt46, 
										/*41*/		charag_9165, charag_9166, charag_9168, charag_9167, charag_Del, 
										/*46*/		charag_Ins, charag_Pgup, charag_Pgon, charag_Home, charag_End, 
													};

int match_alt_table[8] = { 98, 102, 114, 108, 117, 112, 5, 46};  //

int match_dre_table[6] = { 65, 66, 68, 67, 72, 70};    //

int match_dpi_table[4] = { 51, 50, 53, 54}; //

char ctrlW_deleteword[MAXCOMMAND_SIZE];

int shiftjudeg(int matchsource,char matchcode)   // if success return next matchsource or function number
// -1 : fail    0 : next   >0 : success
{	
	int num = 0;
	if(27 == matchsource)
	{
		if (91 == matchcode)   // Need to be paired again
		{
			return 0;
		}
		else if (-1 != (num = in_table(match_alt_table,8,matchcode)))   //Pairing success
		{
			return num + 33;
			/* code */
		}
		else     // pariing failure
		{
			return -1;
			/* code */
		}
	}
	else if (91 == matchsource)
	{
		if(-1 != (num = in_table(match_dre_table,6,matchcode)))   //Pairing success
		{
			if(num < 4)
			{
				return num + 41;
			}
			else
			{
				return num + 45;
			}
		}
		else if( -1 != (num = in_table(match_dpi_table,4,matchcode)) )   // Need to be paired again
		{
			return 0;
		}
		else // pariing failure
		{
			return -1;
		}
		/* code */
	}
	else if(-1 != (num = in_table(match_dpi_table,4,matchsource)))   // 
	{
		if(126 == matchcode)
		{
			return num +45; 
		}
		else // pariing failure
		{
			return -1;
		}
	}
	return -1;
}

int in_table(int* table,int table_size,char code)   //Array subscript
{
	int i = 0;
	if(NULL == table)
	{
		return -1;
	}

	for(i = 0; i < table_size; i++)
	{
		if(code == table[i])
		{
			return i;
		}
	}

	return -1;
}

//   return 0 normal    return -1 over input      return -2 over input and norun this command

int charag_ctrlA(command_type* command) 
{
	int upleng = 0;
	int disleng = disline_realleng;
	if(NULL == command)
	{
		return -1;
	}
	upleng = (command->cursorlocal +  disleng) / windows_info.ws_col;
	translational_cursor(windows_info.ws_col,1);
	translational_cursor(upleng,3);
	translational_cursor(strlen(disline),2);
	command->cursorlocal = 0;
	command->cursor_position.x = 0;
	command->cursor_position.y = 1;
	command->line_position.x = 0;
	//updatecursor(command,0);
	//updateother(command);
	return 0;
	//printf("\033[nD");
}


int charag_ctrlB(command_type* command)
{
	int disleng = disline_realleng;
	int all_leng = command->cursorlocal + disleng;

	if(NULL == command)
	{
		return -1;
	}
	if(command->cursorlocal == 0)
	{
		return 0;
	}
	else
	{
		/*if(all_leng % windows_info.ws_col == 0)   // cursor on left boundary
		{
			command->cursorlocal--;
			command->cursor_position.y = (command->cursorlocal +  disleng) / windows_info.ws_col + 1;
			command->cursor_position.x = command->cursor_position.y == 1 ? 
										 windows_info.ws_col - disleng - 1 : windows_info.ws_col - 1; 
			command->line_position.x--;
			//print
			write(STDOUT_FILENO,"\033[1A",4);
			translational_cursor(windows_info.ws_col - 1,2);

		}
		else
		{
			write(STDOUT_FILENO,"\033[1D",4);
			command->cursor_position.y = (command->cursorlocal +  disleng) / windows_info.ws_col + 1;
			command->cursor_position.x = command->cursor_position.y == 1 ? 
										 windows_info.ws_col - disleng - 1 : windows_info.ws_col - 1;
			command->line_position.x--;
		}*/
		updatecursor(command,command->cursorlocal - 1);
		

	}
	return 0;
}

int charag_ctrlC(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
	//print
	write(STDOUT_FILENO,"^C",2);
	//send signal
	//supervisor process
	return -4;
}

int charag_ctrlD(command_type* command)
{
	int leng = strlen(command->command_content + command->cursorlocal + 1);
	char buf[MAXCOMMAND_SIZE];

	if(NULL == command)
	{
		return -1;
	}
	if(command->size == 0)
	{
		return 0;
	}
	if(command->cursorlocal == command->size)
	{
		return 0;
	}
	// print
	string_translation(command->command_content,command->cursorlocal + 1, -1);
	command->command_content[command->size - 1] = ' ';
	write_and_back(STDOUT_FILENO,command,command->command_content + command->cursorlocal);
	//set command
	command->command_content[command->size - 1] = '\0';
	command->linesize[command->line - 1]--;
	command->size--;
	if((strlen(disline) + command->size) % windows_info.ws_col == 0)
	{
		command->screenline--;
		command->slinesize[command->screenline] = 0;
	}


	return 0;
}

int charag_ctrlE(command_type* command)
{
	int right_back = 0;
	int downleng = 0;
	int disleng = disline_realleng;

	if(NULL == command)
	{
		return -1;
	}
	if(command->cursorlocal == command->size)
	{
		return 0;
	}
	/*//set command
	command->cursor_position.y = (command->size + disleng) / windows_info.ws_col - (command->cursorlocal + disleng) / windows_info.ws_col;
	command->cursor_position.x = (command->cursorlocal + disleng);
	command->line_position.x = command->size;
	command->cursorlocal = command->size;
	//print
	downleng = (command->size + disleng) / windows_info.ws_col - (command->cursorlocal + disleng) / windows_info.ws_col;
	command->screenline = (command->size + disleng - 1) / windows_info.ws_col + 1; 
	translational_cursor(downleng ,4);
	translational_cursor(windows_info.ws_col,1);
	right_back = command->cursor_position.y == 1 ? 
				 strlen(disline) + command->size : command->slinesize[command->screenline - 1];
	translational_cursor(right_back,2);*/
	updatecursor(command,command->size);
	//updateother(command);
	//*****
	return 0;
}

int charag_ctrlF(command_type* command)
{
	int disleng = disline_realleng;
	int all_leng = command->cursorlocal + disleng;

	if(NULL == command)
	{
		return -1;
	}
	if(command->cursorlocal >= command->size)
	{
		return 0;
	}
	else
	{
		if ((all_leng + 1) % windows_info.ws_col == 0)
		{
			translational_cursor(windows_info.ws_col,1);
			translational_cursor(1,4);
			command->cursor_position.y++;
			command->cursor_position.x = 0;
		}
		else
		{
			write(STDOUT_FILENO,"\033[1C",4);
			command->cursor_position.x++;
		}
	}
	command->line_position.x++;
	command->cursorlocal++;
	return 0;
}

int charag_ctrlG(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
}

int charag_ctrlH(command_type* command)
{
	int leng = strlen(command->command_content + command->cursorlocal);
	if(NULL == command)
	{
		return -1;
	}
	if(0 == command->size)
	{
		return 0;
	}
	if(command->cursorlocal == 0)
	{
		return 0;
	}
	//
	string_translation(command->command_content,command->cursorlocal,-1);
	command->command_content[command->size - 1] = ' ';
	charag_ctrlB(command);   // be carefor the member of command  has been change
	//print
	write_and_back(STDOUT_FILENO,command,command->command_content + command->cursorlocal);
	//set command
	command->command_content[command->size - 1] = '\0';
	command->size--;
	command->linesize[command->line - 1]--;
	command->slinesize[command->screenline - 1]--;
	if((strlen(disline) + command->size) % windows_info.ws_col == 0)
	{
		command->screenline--;
	}
	//command->cursorlocal--;
	return 0;
}

int charag_ctrlI(command_type* command)   //tab
{
	static int times = 0;

	if(NULL == command)
	{
		return -1;
	}
	handle_tab(command);
	//****************
	return 0;
}

int charag_ctrlJ(command_type* command)   // It is \r  10
{
	if(NULL == command)
	{
		return -1;
	}
}

int charag_ctrlK(command_type* command)
{
	char bufblank[MAXCOMMAND_SIZE];
	int i = 0;
	int j = 0;

	if(NULL == command)
	{
		return -1;
	}
	if(0 == command->size)
	{
		return 0;
	}
	if(command->size == command->cursorlocal)
	{
		return 0;
	}
	//get nubmer of blank
	for(i = 0, j = command->cursorlocal; j < command->size; i++, j++)
	{
		bufblank[i] = ' ';
	}
	bufblank[i] = '\0';
	//set command
	command->command_content[command->cursorlocal] = 0;
	command->size = command->cursorlocal;
	command->linesize[command->line - 1] = command->size;
	for(i = command->screenline; i < command->cursor_position.y ; i--)
	{
		command->slinesize[i - 1] = 0;
	}
	command->screenline = command->cursor_position.y;
	command->slinesize[command->screenline - 1] = command->cursor_position.x;
	//print
	write_and_back(STDOUT_FILENO,command,bufblank);

	return 0;
}

int charag_ctrlL(command_type* command)
{
	int i = 0;
	char* buf = NULL;
	int real_x = 0;
	int disleng = disline_realleng;
	int all_leng = 0 ;
	int upleng = 0;
	int real_y = 0;

	if(NULL == command)
	{
		return -1;
	}
	buf = (char*)malloc((windows_info.ws_row*2 + 1)*sizeof(char));
	//print '\n'
	for( i = 0; i < windows_info.ws_row; i++)
	{
		buf[2*i] = '\r';
		buf[2*i + 1] = '\n';
	}
	buf[i] = '\0';
	write(STDOUT_FILENO,buf,windows_info.ws_row*2);
	//set cursor
	translational_cursor(windows_info.ws_row - 1,3);
	// print disline
	display_linehead();
	write(STDOUT_FILENO,command->command_content,command->size);
	all_leng = command->size + disleng;
	if((all_leng) % windows_info.ws_col == 0)
	{
		write(STDOUT_FILENO,"\r\n",2);
	}
	// set cursor again
	real_y = (command->cursorlocal + disleng) / windows_info.ws_col;
	upleng = all_leng / windows_info.ws_col - real_y;
	if(command->cursorlocal != command->size)
	{
		real_x = command->cursor_position.y == 1 ? 
			 	 disleng + command->cursor_position.x : command->cursor_position.x;
		if(command->screenline > command->cursor_position.y)
		{
			translational_cursor(upleng,3);
		}
		translational_cursor(windows_info.ws_col,1);
		translational_cursor(real_x,2);
		
	}
	free(buf);
	return 0;
}

int charag_ctrlM(command_type* command)  //13
{
	if(NULL == command)
	{
		return -1;
	}
}

int charag_ctrlN(command_type* command)   //with history
{
	if(NULL == command)
	{
		return -1;
	}
	return charag_9166(command);
}

int charag_ctrlO(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
	return -2;
}


int charag_ctrlP(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
	return charag_9165(command);
}


int charag_ctrlQ(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}

	//write(STDOUT_FILENO,"\r\nin ctrlQ\r\n",12);
	screenlock = 0;

	return 0;
}


int charag_ctrlR(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
	// set cursol
	//updatecursor(command,command->size);
	//write(STDOUT_FILENO,"\n",1);
	// 
	return 0;
}


int charag_ctrlS(command_type* command)	// screenlock
{
	if(NULL == command)
	{
		return -1;
	}

	//write(STDOUT_FILENO,"\r\nin ctrls\r\n",12);

	screenlock = 1;

	return 0;
}


int charag_ctrlT(command_type* command)
{
	char temp = 0;
	char buf[3];

	if(NULL == command)
	{
		return -1;
	}
	if(command->size == 0)
	{
		return 0;
	}
	if(command->cursorlocal == command->size || command->cursorlocal == 0)
	{
		return 0;
	}

	temp = command->command_content[command->cursorlocal];
	buf[0] = temp;
	command->command_content[command->cursorlocal] = command->command_content[command->cursorlocal - 1];
	buf[1] = command->command_content[command->cursorlocal];
	command->command_content[command->cursorlocal - 1] = temp;
	buf[2] = '\0';

	charag_ctrlB(command);
	write_and_back(STDOUT_FILENO,command,buf);
	charag_ctrlF(command);

	return 0;
}


int charag_ctrlU(command_type* command)
{
	int i = 0;
	int allsize = 0;
	int cursor = 0;
	int nowsize = 0;

	if(NULL == command)
	{
		return -1;
	}
	if(command->size == 0 || 0 == command->cursorlocal)
	{
		return 0;
	}
	// set command_content
	allsize = command->size;
	cursor = command->cursorlocal;
	string_translation(command->command_content,cursor,-cursor);
	nowsize = strlen(command->command_content);
	for(i = nowsize; i < allsize; i++)
	{
		command->command_content[i] = ' ';
	}
	command->command_content[i] = '\0';
	charag_ctrlA(command);
	//print
	write_and_back(STDOUT_FILENO,command,command->command_content);
	//set command again
	command->size = nowsize;
	command->command_content[nowsize] = '\0';
	command->linesize[command->line-1] = nowsize;
	setsrceen_from_size_cursor(command);  

	return 0;
}


int charag_ctrlV(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}

}


int charag_ctrlW(command_type* command)
{
	int i = 0;
	int all_leng = 0;
	int newsize = 0;
	int delete_leng = 0;
	int newlocal = 0;
	int oldlocal = 0;

	if(NULL == command)
	{
		return -1;
	}
	if(command->size == 0 || 0 == command->cursorlocal)
	{
		return 0;
	}
	//get delete word
	oldlocal = command->cursorlocal;
	charag_altB(command);
	delete_leng = oldlocal - command->cursorlocal;
	for(i = 0; i < delete_leng ; i++)
	{
		ctrlW_deleteword[i] = command->command_content[command->cursorlocal + i];
	}
	ctrlW_deleteword[i] = '\0';
	// delete word
	string_translation(command->command_content, oldlocal, -delete_leng);
	newsize = command->size	- delete_leng;
	for(i = newsize; i < command->size; i++)
	{
		command->command_content[i] = ' ';
	}
	//print
	write_and_back(STDOUT_FILENO,command,command->command_content + command->cursorlocal);
	// set size
	command->size = newsize;
	command->linesize[command->line - 1 ] -= delete_leng;
	//setsrceen_from_size_cursor(command);
	setsrsize_from_size(command);

	return 0;
}


int charag_ctrlX(command_type* command)
{
	static int cur1 = 0;
	static int cur2 = 0;
	static int times = 0;
	command_type null_command;

	if(NULL == command)
	{
		return -1;
	}
	times++;
	cur1 = command->cursorlocal;
	if(stack_command.sp > 1)
	{	
		// because sp - 1 is this push state
		cur2 = stack_command.stack_body[stack_command.sp - 2].cursorlocal;
	}
	if(1 == times)
	{
		pop_command(&stack_command,&null_command);
	}
	if(2 == times)
	{
		times = 0;
		updatecursor(command,cur2);
	}

	return 0;
}


int charag_ctrlY(command_type* command)  // Insret the last ctrl+w word at the cursor
{
	int i = 0;
	if(NULL == command)
	{
		return -1;
	}
	for(i = 0; ctrlW_deleteword[i] != '\0' ; i++)
	{
		addiscode_to_command(command,ctrlW_deleteword[i]);
		printchar(command,ctrlW_deleteword[i]);
	}
	//printf("\n%s\n", ctrlW_deleteword);
	return 0;
}


int charag_ctrlZ(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
	write(STDOUT_FILENO,"^Z",2);
	return -4;
}


int charag_ctrl27(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
}


int charag_ctrl28(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
	
}


int charag_ctrl29(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
}

int charag_ctrl30(command_type* command)
{

}

int charag_ctrl31(command_type* command)
{
	command_type temp_command;
	char space[MAXCOMMAND_SIZE];
	int command_leng = 0;
	int i = 0;
	int disleng = disline_realleng;
	int right_back = 0;
	//int cursorlocal = 0;

	if(NULL == command)
	{
		return -1;
	}
	clearcommand(&temp_command);
	pop_command(&stack_command,&temp_command);
	if(-1 == pop_command(&stack_command,&temp_command))
	{
		return 0;
	}
	// get space and flash
	command_leng = command->size;
	for(i = 0; i < command_leng ; i++)
	{
		space[i] = ' ';	
	}
	space[i] = '\0';
	// set command
	clearcommand(command);
	write_and_back(STDOUT_FILENO,command,space);
	command_copy(command,&temp_command);
	temp_command.cursorlocal = 0;
	write_and_back(STDOUT_FILENO,&temp_command,command->command_content);
	// printf
	/*right_back = command->cursor_position.y == 1 ? 
				 disleng + command->cursor_position.x : command->cursor_position.x;
	translational_cursor(command->cursor_position.y - 1,4);
	translational_cursor(windows_info.ws_col,1);
	translational_cursor(right_back,2);*/
	updatecursor(command,command->cursorlocal);

	return 0;	
}


int charag_ctrl32(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
}

int charag_altB(command_type* command)
{
	int i = 0;
	//printf("\r\nin\r\n");
	if(NULL == command)
	{
		return -1;
	}
	if(command->size == 0 || 0 == command->cursorlocal)
	{
		return 0;
	}
	i = command->cursorlocal - 1;
	while( command->command_content[i] == ' ')
	{
		i--;
		command->line_position.x--;
	}
	for( ; i>=0 && command->command_content[i] != ' ' ; i-- )
	{
		command->line_position.x--;
	}
	//command->cursorlocal = i + 1;
	updatecursor(command,i + 1);

	return 0;
}

int charag_altF(command_type* command)
{
	int i = 0;
	if(NULL == command)
	{
		return -1;
	}
	if(0 == command->size || command->size == command->cursorlocal )
	{
		return 0;
	}
	// Skip the space
	i = command->cursorlocal;
	while( command->command_content[i] == ' ' )
	{
		i++;
	}
	//
	while( i < command->size && command->command_content[i] != ' ')
	{
		i++;
	}
	command->line_position.x += i - command->cursorlocal;
	//command->cursorlocal = i;	
	updatecursor(command,i);

	return 0;
}



int charag_altR(command_type* command)
{
	int i = 0;
	char buf_space[MAXCOMMAND_SIZE];

	if(NULL == command)
	{
		return -1;
	}
	if(0 == command->size)
	{
		return 0;
	}
	for(i = 0; i < command->size; i++)
	{
		buf_space[i] = ' ';
	}
	buf_space[i] = '\0';
	//command->cursorlocal = 0;
	updatecursor(command,0);
	write_and_back(STDOUT_FILENO,command,buf_space);
	//clearcommand(command);
	clearline(command);

	return 0;
}


int charag_altL(command_type* command)
{
	int i = 0;
	int j = 0;
	char buf[MAXCOMMAND_SIZE];

	if(NULL == command)
	{
		return -1;
	}
	if(0 == command->size)
	{
		return 0;
	}
	// skip
	i = command->cursorlocal;
	j = 0;
	while(i < command->size && command->command_content[i] == ' ')
	{
		i++;
		buf[j] = ' ';
		j++;
	}
	//get string
	for(; i < command->size && command->command_content[i] != ' '; i++ , j++)
	{
		if(command->command_content[i] >= 65 && command->command_content[i] <= 90)
		{
			command->command_content[i] += 32;
		}
		buf[j] = command->command_content[i];
	}
	buf[j] = '\0';
	//print
	write_and_back(STDOUT_FILENO,command,buf);
	// 
	//command->cursorlocal = i;
	updatecursor(command,i);

	return 0;
}


int charag_altU(command_type* command)
{
	int i = 0;
	int j = 0;
	char buf[MAXCOMMAND_SIZE];

	if(NULL == command)
	{
		return -1;
	}
	if(0 == command->size)
	{
		return 0;
	}
	// skip
	i = command->cursorlocal;
	j = 0;
	while(i < command->size && command->command_content[i] == ' ')
	{
		i++;
		buf[j] = ' ';
		j++;
	}
	//get string
	for(; i < command->size && command->command_content[i] != ' '; i++ , j++)
	{
		if(command->command_content[i] >= 97 && command->command_content[i] <= 122)
		{
			command->command_content[i] -= 32;
		}
		buf[j] = command->command_content[i];
	}
	buf[j] = '\0';
	//print
	write_and_back(STDOUT_FILENO,command,buf);
	// 
	//command->cursorlocal = i;
	updatecursor(command,i);

	return 0;
}

int charag_altP(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
}


int charag_alt46(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
}


int charag_acE(command_type* command)
{
	char temp_content[MAXCOMMAND_SIZE];
	if(NULL == command)
	{
		return -1;
	}
	strcpy(temp_content,command->command_content);
	updatecursor(command,command->size);
	charag_ctrlU(command);
	develop_alias(temp_content);
	strcpy(command->command_content,temp_content);
	command->size = strlen(temp_content);
	command->cursorlocal = command->size;
	printf_command_withcursor(command);
	return 0;
}

int charag_9165(command_type* command)
{
	command_type temp_command;
	if(NULL == command)
	{
		return -1;
	}
	if(-1 != get_last_history(&temp_command))
	{
		charag_altR(command);
		command_copy(command,&temp_command);
		write(STDOUT_FILENO,command->command_content,command->size);
	}
	return 0;
}

int charag_9166(command_type* command)
{
	command_type temp_command;
	if(NULL == command)
	{
		return -1;
	}
	if(-1 != get_next_history(&temp_command))
	{
		charag_altR(command);
		command_copy(command,&temp_command);
		write(STDOUT_FILENO,command->command_content,command->size);
	}
	return 0;
}

int charag_9168(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
	charag_ctrlB(command);
	return 0;
}

int charag_9167(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
	charag_ctrlF(command);
	return 0;
}

int charag_Del(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
	charag_ctrlD(command);
	return 0;
}

int charag_Ins(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
	charag_ctrlV(command);
	return 0;
}

int charag_Home(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
	charag_ctrlA(command);
	return 0;
}

int charag_Pgup(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
}

int charag_Pgon(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
}

int charag_End(command_type* command)
{
	if(NULL == command)
	{
		return -1;
	}
	charag_ctrlE(command);
	return 0;
}
