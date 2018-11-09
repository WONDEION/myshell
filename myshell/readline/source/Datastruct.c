#include "Datastruct.h"

// 目录尾部不加 '/'
uid_t uid;
directory_type home;
directory_type workdir;
command_stack stack_command;     
char begindir[DIRECTORY_SIZE];
char nowdir[DIRECTORY_SIZE];
char programdir[DIRECTORY_SIZE];
struct passwd *user_inf;    // User information
Memory_table memory;       // Malloc all information tables
struct winsize windows_info;   // Window information
struct passwd* user_inf;
command_type* nowcommand_point;
environment_variable user_bgdir;
envtable msh_envtable;
stack_char_type stack_char;
stack_int_type stack_int;
int display_open = 1;


char disline_normal[DIRECTORY_SIZE + MAXNAME_SIZE] = {'m','y','s','h','e','l','l','@','\0'};
char disline_again[2] = ">";
char disline[DIRECTORY_SIZE + MAXNAME_SIZE];
int disline_realleng = 0;
int disline_choice = 0;  // 
int dirchange = 0;
int disline_dirloc = 0;

// state variable 
int on_read = 0; // 1 is in readline
int bg_lock = 0;
int due_char = 0;
 int havesignal = 0;


int clear_commandstack(command_stack* pstack)
{
	int i = 0;
	for(i = 0 ; i < pstack->sp ; i++)
	{
		clearcommand(&(pstack->stack_body[i]));
	}
	pstack->sp = 0;
	return 0;
}

int push_command(command_stack* pstack,command_type* pcommand)
{
	if( MAXSTACK_SIZE == pstack->sp)
	{
		return -1;
	}
	pcommand->pushstack_times++;
	clearcommand(&(pstack->stack_body[pstack->sp]));
	command_copy(&(pstack->stack_body[pstack->sp]),pcommand);
	pstack->sp++;
	return pstack->sp;
}

int pop_command(command_stack* pstack,command_type* pcommand)
{
	if(0 == pstack->sp)
	{
		return -1;
	}
	clearcommand(pcommand);
	command_copy(pcommand,&(pstack->stack_body[pstack->sp - 1]));
	pstack->sp--;
	clearcommand(&(pstack->stack_body[pstack->sp]));
	return pstack->sp;
}

int push(stack* pstack,void* source)
{

}

int pop(stack* pstack,void* dest)
{

}

int push_char(char ch)
{
	if(stack_char.sp == MAXSTACK_SIZE)
	{
		return -1;
	}
	stack_char.content[stack_char.sp] = ch;
	stack_char.sp++;
	return 0;
}

char pop_char()
{
	if(stack_char.sp == 0)
	{
		return -1;
	}
	stack_char.sp--;
	return stack_char.content[stack_char.sp];
}

int push_int(stack_int_type* pstack, int digit)
{
	if(pstack->sp == MAXSTACK_SIZE)
	{
		return -1;
	}
	pstack->content[pstack->sp] = digit;
	pstack->sp++;
	return 0;
}

int pop_int(stack_int_type* pstack, int* pdigit)
{
	if(pstack->sp == 0)
	{
		return -1;
	}
	pstack->sp--;
	*pdigit = pstack->content[pstack->sp];
	return 0;
}

int is_brace_matching(char* string)
{
	int sp = stack_char.sp;
	int i = 0;
	if(NULL == string)
	{
		return -1;
	}
	for(i = 0 ; string[i] != '\0';i++)
	{
		//push char
		if('(' == string[i] || '{' == string[i] || '[' == string[i])
		{
			if('(' == string[i])
			{
				push_char(string[i] - 1);
			}
			else
			{
				push_char(string[i]);
			}
		}
		if(')' == string[i] || '}' == string[i] || ']' == string[i])
		{
			if(stack_char.content[stack_char.sp - 1] + 2 == string[i])
			{
				pop_char();
			}
			else
			{
				if('(' == string[i])
				{
					push_char(string[i] - 1);
				}
				else
				{
					push_char(string[i]);
				}
			}
		}
	}
	if(sp == stack_char.sp)
	{
		return 1;
	}
	stack_char.sp = sp;
	return 0;
}

int isbrace(char ch)
{
	if(ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']')
	{
		return 1;
	}
	return 0;
}

int is_quotes_match(char* string)
{
	int sp = 0;
	int i = 0;
	int single_quotes = 0;
	int double_quotes = 0;
	sp = stack_char.sp;
	while( '\0' != string[i] )
	{
		if('\'' == string[i])
		{
			if(0 == single_quotes)
			{
				push_char(string[i]);
				single_quotes++;
			}
			else
			{
				if('\'' == stack_char.content[stack_char.sp - 1])
				{
					pop_char();
					single_quotes--;
				}
				else
				{
					push_char(string[i]);
					single_quotes++;
				}
			}
		}
		else if('\"' == string[i])
		{
			if(0 == double_quotes)
			{
				push_char(string[i]);
				double_quotes++;
			}
			else
			{
				if('\"' == stack_char.content[stack_char.sp - 1])
				{
					pop_char();
					double_quotes--;
				}
				else
				{
					push_char(string[i]);
					double_quotes++;
				}
			}
		}
		i++;
	}
	if(sp == stack_char.sp)
	{
		return 1;
	}
	stack_char.sp = sp;
	return 0;
}

int is_commandspacer(char* string)  // return how many char need to step
{
	if('\0' == string[0])
	{
		return -1;
	}
	if(';' == string[0])
	{
		return 1;
	}
	if('|' == string[0])
	{
		if('|' == string[1])
		{
			return 2;
		}
		return 1;
	}
	if('&' == string[0])
	{
		if('&'== string[1])
		{
			return 2;
		}
		return 0;
	}
	return 0;
}

char** malloc_arg(int n)
{
	char** arg;
	int i = 0;
	if(n == 0)
	{
		return NULL;
	}
	arg = (char**)malloc((n + 1) * sizeof(char*));
	for(i = 0; i < n; i++)
	{
		arg[i] = (char*)malloc(DIRECTORY_SIZE * sizeof(char));
	}
	arg[i] = NULL;
	return arg;
}

int free_arg(char** arg)
{
	int i = 0;
	if(NULL == arg)
	{
		return 0;
	}
	for(i = 0; arg[i] != NULL; i++)
	{
		free(arg[i]);
	}
	free(arg);
	return 0;
}

void* msh_malloc(unsigned int size_bytes)
{
	// procss.c have one malloc
}

void msh_free(void* fp)
{

}

int command_copy(command_type* pdest,command_type* psource)
{
	if(psource == NULL || pdest == NULL)
	{
		return -1;
	}

	int i = 0;

	pdest->size = psource->size;
    pdest->cursorlocal = psource->cursorlocal;
    pdest->cursor_position.x = psource->cursor_position.x;
    pdest->cursor_position.y = psource->cursor_position.y;
    pdest->line_position.x = psource->line_position.x;
    pdest->line_position.y = psource->line_position.y;
    pdest->line = psource->line;
    pdest->screenline = psource->screenline;

    for(i = 0; i < psource->screenline; i++)
    {
    	pdest->slinesize[i] = psource->slinesize[i];
    }
    for(i = 0; i < psource->line; i++)
    {
    	pdest->linesize[i] = psource->linesize[i];
    }

     strcpy(pdest->command_content,psource->command_content);

     return 0;
}

int addiscode_to_command(command_type *pcommand,char inputchar)
{
	char buf[MAXCOMMAND_SIZE];
	int i = 0;
	int first_leng = windows_info.ws_col - strlen(disline);
	int other_leng = windows_info.ws_col;
	int totalsize = 0;

	if(pcommand->size + 1  >= MAXCOMMAND_SIZE)
	{
		return -1;
	}

	if(pcommand->size == 0)
	{
		pcommand->line = 1;
		pcommand->line_position.y = 1;
		pcommand->screenline = 1;
		pcommand->cursor_position.y = 1;
		pcommand->command_content[pcommand->cursorlocal] = inputchar;
		pcommand->command_content[pcommand->cursorlocal + 1] = '\0'; 
	}
	else
	{

		string_translation(pcommand->command_content,pcommand->cursorlocal,1);

		pcommand->command_content[pcommand->cursorlocal] = inputchar;
	
	// set size
		if(pcommand->size <= first_leng)
		{
			if(pcommand->size == first_leng)      // add number of line 
			{
				pcommand->slinesize[pcommand->screenline] = 0; //because at function last will be add
				pcommand->screenline++;
			}
			else
			{
				
			}
		}
		else
		{	
			totalsize = first_leng;
			for(i = 1; pcommand->size > totalsize; i++)
			{
				totalsize = totalsize + other_leng;
			}
			if(pcommand->size == totalsize)    // add number of line 
			{
				pcommand->slinesize[pcommand->screenline] = 0;  //because at function last will be add 
				pcommand->screenline++;
			}	

		}
	// set cursor
		if(pcommand->cursorlocal + 1 <= first_leng)
		{
			if(pcommand->cursorlocal + 1 == first_leng)
			{
				pcommand->cursor_position.x = -1; //because at function last will be add
				pcommand->cursor_position.y++;
			}
			else
			{
			}
		}
		else
		{
			totalsize = first_leng;
			
			for(i = 1; pcommand->cursorlocal + 1 > totalsize; i++)
			{
				totalsize = totalsize + other_leng;
			}
			if(pcommand->cursorlocal + 1 == totalsize)    // add number of line 
			{
				pcommand->cursor_position.x = -1; //because at function last will be add
				pcommand->cursor_position.y++;
			}
		}


	}

	pcommand->slinesize[pcommand->screenline - 1 ]++;
	pcommand->cursorlocal++;
	pcommand->cursor_position.x++;
	pcommand->line_position.x++;
	pcommand->size++; 
	pcommand->linesize[pcommand->line - 1]++;

	return 0;
}

int printchar(command_type* pcommand,char inputchar)  //printf is before add code 
{
	int disleng = strlen(disline);
	int all_leng = pcommand->cursorlocal + disleng;


	if(NULL == pcommand)
	{
		return -1;
	}

	if(pcommand->size == pcommand->cursorlocal)
	{
		//msh_fprintf(printfd,"%c",inputchar);
		write(STDOUT_FILENO,&inputchar,1);
		if((all_leng) % windows_info.ws_col == 0)
		{
			write(STDOUT_FILENO," \b",2);
		}
	}
	else 
	{
		write(STDOUT_FILENO,&inputchar,1);
		write_and_back(STDOUT_FILENO, pcommand, pcommand->command_content+pcommand->cursorlocal);
		//write(STDOUT_FILENO,"\033[1C",4);
	}
	
	return 0;
}

int clearcommand(command_type* pcommand)  
{
	int i = 0;

	pcommand->command_content[0] = 0;
	pcommand->size = 0;
	pcommand->cursorlocal = 0;
	pcommand->line = 0;
	pcommand->screenline = 0;
	pcommand->cursor_position.x = 0;
	pcommand->cursor_position.y = 1;
	pcommand->line_position.x = 0;
	pcommand->line_position.y = 1;
	pcommand->pushstack_times = 0;

	for(i = 0; i < MAXCOMMAND_LINE; i++)
	{
		pcommand->linesize[i] = 0;
		pcommand->slinesize[i] = 0;
	}
	return 0;
}	

int clearline(command_type* pcommand)
{
	int i = 0;
	if( NULL == pcommand)
	{
		return 0;
	}
	pcommand->size = pcommand->size - pcommand->linesize[pcommand->line - 1];
	pcommand->linesize[pcommand->line - 1] = 0;
	pcommand->cursorlocal = 0;
	pcommand->line = 0;
	pcommand->screenline = 0;
	pcommand->cursor_position.x = 0;
	pcommand->cursor_position.y = 1;
	pcommand->line_position.x = 0;
	pcommand->line_position.y = pcommand->line_position.y == 1 ? 1 : pcommand->line_position.y - 1;
	pcommand->pushstack_times = 0;
	for(i = 0; i < MAXCOMMAND_LINE; i++)
	{
		pcommand->slinesize[i] = 0;
	}
	return 0;
}

int combinecommand(command_type* paim, command_type* psource)
{
	int i = 0;
	int j = 0;

	if(NULL == paim || NULL == psource)
	{
		return -1;
	}

	if(paim->size == 0)  // copy
	{
      command_copy(paim,psource);
	}
	else //  combine
	{
		if(paim->size + psource->size > MAXCOMMAND_SIZE)
		{
			return -1;
		}
		paim->cursorlocal = psource->cursorlocal + 1;
		paim->cursor_position.x = psource->cursor_position.x;
     	paim->cursor_position.y = psource->cursor_position.y + paim->screenline;
		
		paim->line_position.x = psource->line_position.x;
     	paim->line_position.y = psource->line_position.y + paim->line;

		paim->line += psource->line;
		paim->command_content[paim->size] = ' ';

		for( i = paim->screenline ,j = 0; j < psource->screenline; i++,j++)
		{
			paim->slinesize[i] = psource->slinesize[j]; 
		}
		for( i = paim->line, j = 0; j <  psource->line; i++, j++)
		{
			paim->linesize[i] = psource->linesize[j];
		}

		paim->screenline += psource->screenline;

		strcpy(paim->command_content + paim->size + 1 , psource->command_content);

		paim->size = paim->size + psource->size + 1;  // the '+1' is for ' '
	}
	paim->command_content[paim->size] = '\0';
	return 0;
}

int write_and_back(int fd, command_type* pcommand, char* string)  // after set command

// print string and Bring the cursor back to its place
{
	int output_number = 0;
	int output_line = 0;
	int right_back = 0;
	int sleng = 0;

	sleng = strlen(string);
	// printf
	write(fd,string,sleng);
	//****
	// Adjust the cursor position
	right_back = pcommand->cursor_position.y == 1?
				 pcommand->cursor_position.x + disline_realleng : pcommand->cursor_position.x;
	output_number = sleng + right_back;
	output_line = (output_number - 1)/windows_info.ws_col;
	translational_cursor(output_line,3);
	translational_cursor(windows_info.ws_col + MAXTERMIO_SIZE,1);
	translational_cursor(right_back,2);
	if(right_back == windows_info.ws_col)
	{
		write(fd,string,1);
		write(fd,"\b",1);
	}

	return 0;
}

int isadd_srline(int add,command_type* pcommand)
{

}

int where_line(int size)
{

}	

int translational_cursor(int num,int dre)  // 1 left  2 right  3 up 4 down
{
	char buf[50] = {'\033','['};
	int i = 0;

	if(num <= 0)
	{
		return 0;
	}

	int_to_string(num,buf+2,10);

	i = strlen(buf);
	switch(dre)
	{
		case 1:	buf[i] = 'D';
				break;
		case 2: buf[i] = 'C';
				break;
		case 3: buf[i] = 'A';
				break;
		case 4: buf[i] = 'B';
				break;
	}
	i++;
	buf[i] = '\0';

	write(STDOUT_FILENO,buf,i);
	return 0;
}

int display_linehead()
{
	
	int i = 8;
	int j = 0;
	char disbuf[512];

	if(1 == disline_choice)  // is no finished
	{
		write(STDOUT_FILENO,"in 1 == disline_choice\n",24);
		strcpy(disline,disline_again);
		write(STDOUT_FILENO,disline,1);
		return 0;
	}

	//if(1 == dirchange)
	//{
		//dirchange = 0;
		strcpy(disline,disline_normal);
		//printf("nowdir : %s\n", nowdir);
		//printf("user_bgdir : %s\n",user_bgdir.value);
		if(0 == strcmp(user_bgdir.value,nowdir))  // if the directory is '~''
		{
			//printf("\n in 0 == strcmp(user_bgdir.value,nowdir) \n");
			i = disline_dirloc;
			disline[i] = '~';
			i++;
		}
		else  //   other
		{
			for(i = disline_dirloc, j = 0; nowdir[j] != 0; i++, j++)
			{
				disline[i] = nowdir[j];
			}
		}
		disline[i] = '$';
		i++;
		disline[i] = ' ';
		i++;
		disline[i] = '\0';
//	}
	//printf("\n%s\n", disline);
	//set color
	//printf("\033[36m%s\033[0m\r\n",disline);
	write(STDOUT_FILENO,"\033[1m",4);
	write(STDOUT_FILENO,"\033[35m",5);
	write(STDOUT_FILENO,disline,disline_dirloc);
	write(STDOUT_FILENO,"\033[34m",5);
	write(STDOUT_FILENO,disline+disline_dirloc,strlen(disline)-disline_dirloc);
	// set back
	write(STDOUT_FILENO,"\033[0m",4); 
	disline_realleng = strlen(disline) % windows_info.ws_col;
	return 0;
}

int setsrsize_from_size(command_type* pcommand)
{
	int i = 0;
	int allleng = 0;
	int srline = 0;
	int disleng = disline_realleng;

	if(NULL == pcommand)
	{
		return -1;
	}
	if(0 == pcommand->size)
	{
		return 0;
	}
	allleng = pcommand->size + disleng;
	srline = (allleng - 1) / windows_info.ws_col + 1;

	for( i =0; i < srline ; i++)
	{
		pcommand->slinesize[srline - 1] = allleng > windows_info.ws_col ? 
										  windows_info.ws_col : allleng;
		allleng = allleng - windows_info.ws_col;
	}
	pcommand->slinesize[0] -= disleng;

	return 0;

}

//!!
int setsrceen_from_size_cursor(command_type* pcommand)  // set screen from command size and cursirlacol
{
	int sr_i = 0;
	int disleng = disline_realleng;
	int allleng = 0;
	int templeng = 0;
	int allleng_cur = 0;

	if(pcommand == NULL)
	{
		return 0;
	}
	if(pcommand->size == 0)
	{
		pcommand->screenline = 0;
		pcommand->slinesize[0] = 0;
		pcommand->cursor_position.y = 0;
		pcommand->cursor_position.x = 0;
		return 0;
	}

	allleng = pcommand->size + disleng;
	templeng = allleng;
	allleng_cur = pcommand->cursorlocal + disleng;
	// set screenline
	pcommand->screenline = (allleng - 1) / windows_info.ws_col + 1;
	//set slinesize
	for(sr_i = 0 ; sr_i < pcommand->screenline; sr_i++)
	{
		pcommand->slinesize[sr_i] = (templeng > windows_info.ws_col ? windows_info.ws_col : templeng);
		templeng = templeng - windows_info.ws_col;
	}
	pcommand->slinesize[0] -= disleng;
	// set screen x and y
	pcommand->cursor_position.y = (allleng_cur)/windows_info.ws_col + 1;
	pcommand->cursor_position.x = allleng_cur % windows_info.ws_col;
	if(pcommand->cursor_position.y == 1)
	{
		pcommand->cursor_position.x -= disleng;
	} 

	return 0;
}

int updatecursor(command_type* pcommand , int newloacl)  // Update the position of the cursor on the screen by command
// pcommand representing the original position, cursorlocal representin the new position
{
	int new_x = 0, new_y = 0;
	int old_x = 0, old_y = 0;
	int disleng = disline_realleng;
	int line_leng = 0;
	int allleng = 0;
	int dre = 0;
	int transnum = 0;

	if(NULL == pcommand)
	{
		return 0;
	}
	new_y = (newloacl + disleng) / windows_info.ws_col + 1;
	new_x = (new_y == 1) ? newloacl + disleng : ((newloacl + disleng) % windows_info.ws_col);
	line_leng = windows_info.ws_col;
	// get old x,y
	allleng = pcommand->cursorlocal + disleng;
	old_x = allleng % line_leng;
	old_y = (allleng) / line_leng + 1;
	// print at screen
	if(new_y != old_y)
	{
		dre = new_y > old_y ? 4 : 3;
		transnum = new_y > old_y ? new_y - old_y : old_y - new_y;
		translational_cursor(transnum , dre);
	}
	if(new_x != old_x)
	{
		dre = new_x > old_x ? 2 : 1;
		transnum = new_x > old_x ? new_x - old_x : old_x - new_x;
		translational_cursor(transnum , dre);
	}
	// set x,y
	pcommand->cursor_position.x = new_y == 1 ? new_x - disleng : new_x ;
	pcommand->cursor_position.y = new_y;
	pcommand->cursorlocal = newloacl;
	pcommand->line_position.x = newloacl;

	return 0;
}

int printf_command_withcursor(command_type* pcommand)
{
	if(NULL == pcommand)
	{
		return -1;
	}
	write(STDOUT_FILENO,pcommand->command_content,strlen(pcommand->command_content));
	updatecursor(pcommand,pcommand->cursorlocal);
	return 0;
}

int updateother(command_type* pcommand)  // change other form size and cursor
{
	int i = 0;
	int disleng = disline_realleng;
	int allleng = 0;
	int deleteleng = 0;
	int linenumber = 0;
	int lx = 0;

	if(pcommand == 0)
	{
		return -1;
	}
	// x , y
	setsrceen_from_size_cursor(pcommand);
	for(linenumber  = 0, i = 0 ; i < pcommand->size; i++)
	{
		pcommand->linesize[linenumber]++;
		lx++;
		if(pcommand->command_content[i] == '\n')
		{
			linenumber++;
			lx = 0;
		}
	}
	linenumber++;
	pcommand->line = linenumber;
	pcommand->line_position.y = linenumber;
	pcommand->line_position.x = lx;

	return 0;
}

int cleardir(directory_type* pdir)
{
	int i = 0;
	if(NULL == pdir)
	{
		return 0;
	}
	for(i = 0; i < DIRECTORY_SIZE ;i++)
	{
		pdir->directory[i] = 0;
	}
	return 0;
}

int dir_assignment(directory_type* pdir ,char* string_dir)
{
	int i = 0;
	if(NULL == pdir || NULL == string_dir)
	{
		return 0;
	}
	for(i = 0 ; string_dir[i] != '\0'; i++)
	{
		pdir->directory[i] = string_dir[i];
	}
	pdir->directory[i] = '\0';
	return 0;
}

int dir_copy(directory_type* pdest, directory_type* psource)
{
	int i = 0;
	if(NULL == pdest || NULL == psource)
	{
		return -1;
	}
	strcpy(pdest->directory,psource->directory);
	return 0;
}

int dir_cat(directory_type* pfirst, directory_type* psecond)
{
	if(NULL == pfirst || NULL == psecond)
	{
		return -1;
	}
	strcat(pfirst->directory,psecond->directory);
	return 0;
}

int dir_catstring(directory_type* pdir, char* string)
{
	if(NULL == pdir || NULL == string)
	{
		return -1;
	}
	strcat(pdir->directory,string);
	return 0;
}

int delete_lastlv_dir(directory_type* pdir)
{
	int i = 0;	
	if(NULL == pdir || pdir->directory[0] != '/')
	{
		return -1;
	}

	if( 1 == strlen(pdir->directory) )
	{
		return 0;
	}

	i = strlen(pdir->directory);
	if(pdir->directory[i] == '/')
	{
		i--;
	}

	while(pdir->directory[i] != '/')
	{
		i--;
	}

	if(i == 0)
	{
		pdir->directory[1] = '\0';
	}
	else
	{
		pdir->directory[i] = '\0';
	}	

	return 0;
}

int delete_lastlv_dir_string(char* dir)
{
	directory_type dirtemp;
	if(NULL == dir)
	{
		return -1;
	}
	strcpy(dirtemp.directory,dir);
	if(-1 == delete_lastlv_dir(&dirtemp))
	{
		printf("delete_lastlv_dir_string error !!\n");
		return -1;
	}
	strcpy(dir,dirtemp.directory);
	return 0;
}