#include "history.h"

int history_choice = 0;
history_table_type history_table;
char* historyfilename = ".msh_hisfile";
directory_type historyfiledir;
int history_file_fd;   //fd 
envtable history_envtable; 

static size_t fileheadleng_number;
static char numstring[] = "number : ";
static int history_i = 0;			// i of history
static int history_buf_mark = 0;    //  end of cache include last record
static int history_end = 0;			// end of history file   include
static int fd_local = 0;
static int history_ser_number = 0;    // it is the number of history record + 1

int inithisfiledir()
{
	directory_type tempdir;
	dir_catstring(&historyfiledir,"/");
	dir_catstring(&historyfiledir,historyfilename);
	dir_copy(&tempdir,&home);
	dir_cat(&tempdir,&historyfiledir);
	dir_copy(&historyfiledir,&tempdir);
	/*write(STDOUT_FILENO,"historyfiledir : ",17);
	write(STDOUT_FILENO,historyfiledir.directory,strlen(historyfiledir.directory));
	write(STDOUT_FILENO,"\r\n",2);*/
	return 0;
}

int initialize_history_table()
{
	int fd = -1;
	int number = 0;
	int i = 0;
	int beginnum = 0;
	
	history_i = 0;
	history_table.size = 0;
	fd = open_and_creat();
	if(-1 == fd)
	{
		write(STDERR_FILENO,"open_and_creat file error !!\r\n",30);
		return -1;
	}

	history_file_fd = fd;
	number = get_nubmer(fd);
	history_end = number;
	//printf("get_nubmer : %d\r\n",number);

	if( -1 == goto_buffer_offest(fd,number,&beginnum))
	{
		write(STDERR_FILENO,"goto_buffer_offest error !!\r\n",29);
		return -1;
	}
	//printf("beginnum : %d\r\n",beginnum);
	history_i = 0;
	if(beginnum != 0)
	{
		//printf("read_file_to_histabl\n");
		read_file_to_histable(fd,beginnum,number); //adjust history_i
		fd_local = number;
		history_ser_number = number + 1;
	}
	else
	{	
		history_ser_number = 0;
	}
	history_buf_mark = history_i - 1;
	//history_choice = history_i - 1;
	history_choice = history_i;
	//printf("history_buf_mark : %d| history_end : %d| history_i :%d|history_ser_number : %d\n", 
	//		history_buf_mark,history_end,history_i,history_ser_number);
	return 0;
}

int open_and_creat()
{
	int fd = -1;
	char buf[NUMBERSTRING_SIZE + 1];

	if(-1 == access(historyfiledir.directory,F_OK))
	{
		write(STDOUT_FILENO,"NO history file!!!\r\n",20);
		fd = open(historyfiledir.directory ,(O_CREAT|O_RDWR));
		if(-1 == fd)
		{
			write(STDOUT_FILENO,"creat history file failed !!\r\n",30);
		}
		else
		{
			chmod(historyfiledir.directory,(S_IRUSR|S_IWUSR|S_IROTH|S_IRGRP));
			write(STDOUT_FILENO,"creat history file success !!\r\n",31);
			write(fd,numstring,strlen(numstring));
			get_number_string(buf,0);
			write(fd,buf,NUMBERSTRING_SIZE);
			write(fd,";",1);
			write(fd,FILE_WRAP_STRING,WRAP_NUMBER);
		}
	}
	else
	{
		fd = open(historyfiledir.directory ,(O_RDWR));
		if(fd == -1)
		{
			write(STDOUT_FILENO,"open history file failed !!\r\n",29);
		}
		else
		{
			write(STDOUT_FILENO,"open history file success !!\r\n",30);
		}
	}
	fileheadleng_number = 9;
	return fd;
}

int close_hisfile()
{
	return close(history_file_fd);
}

int get_last_history(command_type* pdest)
{
	if( 0 < history_choice)
	{
		history_choice--;
		//return -1;
	}
	//printf("history_choice = %d \r\n",history_choice); 
	clearcommand(pdest);
	strcpy(pdest->command_content,history_table.history_info[history_choice].command);
	pdest->size = strlen(pdest->command_content);
	pdest->cursorlocal = pdest->size;
	updateother(pdest);
	//history_choice--;
	/*if(-1 == history_choice && history_i != 0)
	{
		history_choice++;
	}*/

	return 0;
}

int get_next_history(command_type* pdest)
{
	if(history_choice < history_i - 1)
	{
		history_choice++;
		//return -1;
	}
	//printf("history_choice = %d \r\n",history_choice);

	clearcommand(pdest);
	strcpy(pdest->command_content,history_table.history_info[history_choice].command);
	pdest->size = strlen(pdest->command_content);
	pdest->cursorlocal = pdest->size;
	updateother(pdest);
	/*history_choice++;
	if(history_choice == history_i)
	{
		history_choice--;
	}*/

	return 0;
}

int add_to_history(command_type* pcommand)
{
	if( HISTABLE_SIZE == history_i)
	{
		// adjust table
		write_history(history_file_fd, history_buf_mark + 1, HISTABLE_SIZE);
		history_moveto_head(CACHING_LENGTH, HISTABLE_SIZE - 1);
		history_table.size = CACHING_LENGTH;
		history_i = CACHING_LENGTH;
		history_buf_mark = CACHING_LENGTH - 1;
	}
	//printf("history_i : %d  command : %s \n",history_i,pcommand->command_content );
	//write(STDOUT_FILENO,"in add_to_history\r\n",19);
	strcpy(history_table.history_info[history_i].command , pcommand->command_content);
	history_table.history_info[history_i].mark = history_ser_number;
	history_table.size++;
	history_i++;
	history_ser_number++;
	//print_hisinfo(&history_table.history_info[history_i - 1]);

	return 0;
}

int add_and_clearchoice(command_type* pcommand)
{
	if(NULL == pcommand)
	{
		return -1;
	}
	if(0 == pcommand->size || '\0' == pcommand->command_content[0])
	{
		return 0;
	}
	add_to_history(pcommand);
	// Because of its special invocation location
	history_choice = history_i;
	return 0;
}

int write_history(int fd,int hisbegin,int hisend)   // include hisend
{
	int i = 0;
	int ser_number = 0;
	int char_number = 0;
	char buf[MAXCOMMAND_SIZE];

	if(fd == -1)
	{
		return -1;
	}
	if(hisend >= HISTABLE_SIZE || hisbegin < 0 ||  hisend >= history_table.size)
	{
		write(STDERR_FILENO,"write_history : illegal parameter!!\r\n",37);
		return -1;
	}
	if( -1  == lseek(fd,0,SEEK_END))
	{
		write(STDERR_FILENO,"write_history : lseek error!!!\r\n",32);
	}
	for( i = hisbegin ,ser_number = history_end + 1; i <= hisend ; i++ , ser_number++)
	{
		int_to_string(ser_number, buf, 10);
		write(fd,buf,strlen(buf));
		write(fd," ",1);
		char_number = strlen(history_table.history_info[i].command);
		int_to_string(char_number,buf,10);
		write(fd,buf,strlen(buf));
		write(fd," ",1);
		write(fd,history_table.history_info[i].command,char_number);
		write(fd,FILE_WRAP_STRING,WRAP_NUMBER);
	}

	history_end = history_end + hisend - hisbegin + 1;
	fd_local = history_end;

	return 0;
}

int history_moveto_head(int beginnum ,int endnum)   
// Move the command of the selected area to the head     include endnum
{
	int i = 0;
	int j  =0;
	if(endnum >= HISTABLE_SIZE || beginnum < 0)
	{
		return -1;
	}
	for(i = 0 ,j = beginnum; j <= endnum; i++ ,j++)
	{

		history_table.history_info[i].mark = history_table.history_info[j].mark;
		strcpy(history_table.history_info[i].command,history_table.history_info[j].command);
	}
}

int read_file_to_histable(int fd, int beginnum ,int endnum)  // no error judge
// from now fd local
{
	int i = 0;
	int ser_number = 0;
	int char_number = 0;
	char buf[MAXCOMMAND_SIZE];
	int realread = 0;

	if(-1 == fd)
	{
		return -1;
	}
	
	if(endnum > history_end || beginnum < 0)
	{
		return -1;
	}
	//printf("readform file : beginnum %d | end %d\n", beginnum,endnum);
	for( i = beginnum; i < endnum ; i++, history_i++)
	{
		ser_number = get_nextnumber(fd);
		if(-1 == ser_number)
		{
			printf("i : %d\n", i);
			/*lseek(fd,-10,SEEK_CUR);
			realread = read(fd,buf,100);
			write(STDOUT_FILENO,"error at : ", 11);
			write(STDOUT_FILENO,buf,100);
			write(STDOUT_FILENO,"|\n",2);
			lseek(fd,-90,SEEK_CUR);*/
			write(STDERR_FILENO,"in rfth : ser_number\r\n",9+3+10);
			
		}
		char_number = get_nextnumber(fd);
		if(-1 == char_number)
		{
			printf("i : %d\n", i);
			write(STDERR_FILENO,"in rfth : char_number\r\n",9+3+11);
		}
		//read char
		realread = read(fd,buf,char_number);
		buf[realread] = '\0';
		history_table.history_info[history_i].mark = i;
		strcpy(history_table.history_info[history_i].command,buf);
		history_table.size++;
		//step warp
		lseek(fd,WRAP_NUMBER, SEEK_CUR);
	}

	return 0;
}

int get_nubmer(int fd)
{
	off_t offset = 0;
 	char buf[FILEHEAD_SIZE];
 	char buf_c = 0;
 	int i = 0;
 	int number = 0;

	if(-1 == fd)
	{
		return -1;
	}

	offset = lseek(fd,0,SEEK_SET);
	if(-1 == offset)
	{
		write(STDERR_FILENO,"get_nubmer: lseek error!!\r\n",27);
		return -1;
	}
	read(fd,buf,fileheadleng_number);
	buf[fileheadleng_number] = '\0';
	if( 0 != strcmp(buf,numstring))
	{
		write(STDERR_FILENO,"file formatting error !!!\r\n",27);
		return -1;
	}

	i = 0;
	while(buf_c != ';')
	{
		read(fd,&buf_c,1);
		if(!isdigit(buf_c))
		{
			break;
		}
		buf[i] = buf_c;
		i++;
	}
	if(buf_c != ';' && buf_c != ' ')
	{
		write(STDERR_FILENO,"file end formatting error !!!\r\n",31);
		return -1;
	}
	buf[i] = '\0';
	number = atoi(buf);
	lseek(fd,WRAP_NUMBER,SEEK_CUR);

	return number;
}
 
int goto_buffer_offest(int fd,int numberofhis,int* pbegin)  // keep fd at first record which we need read 
// begin is relative to the first record
{
	int number = 0;
	off_t back_offset = 0;
	int step_leng = 0;
	int i = 0;
	int ser_number = 0;
	int char_number = 0;
	off_t offset = 0;

	if(fd < 0 || numberofhis < 0 || NULL == pbegin)
	{
		return -1;
	}
	// keep fd at begin
	if(-1 == (offset = lseek(fd,fileheadleng_number + NUMBERSTRING_SIZE + 1 + WRAP_NUMBER,SEEK_SET)))
	{
		write(STDERR_FILENO,"goto_buffer_offest : lseek error !!!\r\n",38);
		return -1;
	}

	if(0 == numberofhis)
	{
		*pbegin = 0;
		return 0;
	}
	if(numberofhis < CACHING_LENGTH)
	{
		*pbegin = 1;
		return 0;
	}

	step_leng = numberofhis - CACHING_LENGTH;
	/*for(i = 0, back_offset = 0; i < step_leng ; i++)
	{
		//printf("i : %d\n", i);
		ser_number = get_nextnumber(fd);
		back_offset = back_offset + 1 + get_int_bit(ser_number);
		char_number = get_nextnumber(fd);
		back_offset = back_offset + 1 + get_int_bit(char_number);
		lseek(fd, char_number + 1 + WRAP_NUMBER ,SEEK_CUR);
		back_offset = back_offset + char_number + 1 + WRAP_NUMBER;
	}*/
	keephistory_fd_at(fd,step_leng + 1);
	*pbegin = step_leng;
	return back_offset;
}

int get_nextnumber(int fd)   // get next number at file
{
	int i = 0;
	char buf_c = 0;
	char buf[sizeof(int)];
	int ret = 0;

	if(fd < 0)
	{
		return -1;
	}
	//printf("get_nextnumber begin\n");
	do
	{
		read(fd,&buf_c,1);
		//printf("buf_c : %d\n",buf_c);
	}while( ' ' == buf_c );
	
	i = 0;
	if(isdigit(buf_c))
	{
		buf[i++] = buf_c;
		while(buf_c != ' ')
		{
			read(fd,&buf_c,1);
			//printf("buf_c : %d\n",buf_c);
			if(!isdigit(buf_c))
			{
				break;
			}
			buf[i++] = buf_c;
		}
	}
	buf[i] ='\0';
	//printf("get_nextnumber: buf_c : %d  buf_i : %d\r\n",buf_c,strlen(buf) );
	//write(STDERR_FILENO,buf,strlen(buf));
	//printf("buf : %s\r\n",buf);
	if(buf_c != ' ')
	{
		//printf("buf : %s| i : %d\n", buf,i);
		write(STDERR_FILENO,"number formatting error !!!\r\n",29);
		return -1;
	}

	ret = atoi(buf);

	return ret;
}

int close_history()
{
	write_history(history_file_fd, history_buf_mark + 1,history_i -1 );
	updatenumber();
	close_hisfile();
	return 0;
}

int print_histable(int beginnum ,int endnum) //include endnum
{
	int i = 0;
	//write(STDOUT_FILENO,"in print_histable\r\n",19);
	//printf("%d\n", history_table.size);
	if(endnum >= HISTABLE_SIZE || beginnum < 0 || endnum >= history_table.size)
	{
		return -1;
	}
	for(i = beginnum; i<= endnum ; i++)
	{
		//printf("print his table times\n");
		print_hisinfo(&history_table.history_info[i]);
	}

	return i - 1;
}

int print_hisinfo(history_info_type* phis)
{
	char buf[32];
	int ret;
	if(phis == NULL)
	{
		return 0;
	}
	int_to_string(phis->mark,buf,10);
	write(printf_fd,buf,strlen(buf));
	write(printf_fd," ",1);
	write(printf_fd,phis->command,strlen(phis->command));
	write(printf_fd,FILE_WRAP_STRING,WRAP_NUMBER);
	return 0;
}

int updatenumber()
{
	char buf[NUMBERSTRING_SIZE + 1];

	if(history_file_fd == -1)
	{
		return -1;
	}
	lseek(history_file_fd,fileheadleng_number,SEEK_SET);
	get_number_string(buf,history_end);
	write(history_file_fd,buf,NUMBERSTRING_SIZE);
	write(history_file_fd,";",1);
	write(history_file_fd,FILE_WRAP_STRING,WRAP_NUMBER);
	return 0;
}

int get_number_string(char* string,int number)
{
	int i = 0;
	char buf[NUMBERSTRING_SIZE + 1];
	if(string == NULL)
	{
		write(STDERR_FILENO,"get_number_string : NULL error!!!\r\n",35);
		return -1;
	}
	int_to_string(number,buf,10);
	strcpy(string,buf);
	for(i = strlen(buf); i <= NUMBERSTRING_SIZE;i++)
	{
		string[i] = ' ';
	}
	string[i] = '\0';
	return 0;
}

int keephistory_fd_at(int his_fd, int number)
{
	off_t back_offset = 0;
	int step_leng = 0;
	int i = 0;
	int ser_number = 0;
	int char_number = 0;
	off_t offset = 0;

	if(his_fd < 0 || number < 0 || number > history_end)
	{
		return -1;
	}
	// keep fd at begin
	if(-1 == (offset = lseek(his_fd,fileheadleng_number + NUMBERSTRING_SIZE + 1 + WRAP_NUMBER,SEEK_SET)))
	{
		write(STDERR_FILENO,"goto_buffer_offest : lseek error !!!\r\n",38);
		return -1;
	}

	for(i = 1, back_offset = 0; i < number ; i++)
	{
		ser_number = get_nextnumber(his_fd);
		back_offset = back_offset + 1 + get_int_bit(ser_number);
		char_number = get_nextnumber(his_fd);
		back_offset = back_offset + 1 + get_int_bit(char_number);
		lseek(his_fd, char_number /*+ 1*/ + WRAP_NUMBER ,SEEK_CUR);
		back_offset = back_offset + char_number + 1 + WRAP_NUMBER;
	}

	return back_offset;
}

int historyfile_clear()
{

}

int history_print(int beginnum,int endnum)  // include endnum
{
	int i = 0;
	int ser_number = 0 ;
	int char_number = 0;
	char buf[MAXCOMMAND_SIZE];
	if(beginnum < 0 || beginnum >= history_ser_number || beginnum > endnum)
	{
		return 0;
	}
	if(beginnum > history_end - history_buf_mark - 1) // bufmark means 
		// how manty buf
	{	
		print_histable(beginnum - (history_end - 1 - history_buf_mark) - 1
			,endnum -(history_end - 1 - history_buf_mark) - 1);
	}
	else  // need printf flie
	{
		keephistory_fd_at(history_file_fd,beginnum);
		if(endnum > history_end)  // need printf histable
		{
			//int times = 0;
			//printf("bgnum : %d | hisend : %d\n",beginnum, history_end);
			for(i = beginnum; i <= history_end ; i++)
			{
				//times ++;
				//printf("times : %d\n", times);
				ser_number = get_nextnumber(history_file_fd);
				int_to_string(ser_number,buf,10);
				write(printf_fd,buf,strlen(buf));
				write(printf_fd," ",1);
				char_number = get_nextnumber(history_file_fd);
				read(history_file_fd,buf,char_number);
				buf[char_number] = '\0';
				write(printf_fd,buf,char_number);
				lseek(history_file_fd,WRAP_NUMBER, SEEK_CUR);
				write(printf_fd,"\n",1);
			}
			//printf("times : %d\n", times);
			//printf("history_buf_mark: %d\n", history_buf_mark);
			print_histable(history_buf_mark + 1, history_i - 1);
			//printf("this\n");
		}
		else
		{
			for(i = beginnum; i <= endnum ; i++)
			{
				ser_number = get_nextnumber(history_file_fd);
				int_to_string(ser_number,buf,10);
				write(printf_fd,buf,strlen(buf));
				write(printf_fd," ",1);
				char_number = get_nextnumber(history_file_fd);
				read(history_file_fd,buf,char_number);
				buf[char_number] = '\0';
				write(printf_fd,buf,char_number);
				lseek(history_file_fd,WRAP_NUMBER, SEEK_CUR);
				write(printf_fd,"\n",1);
			}
		}
	}
	return 0;
}

int command_history(int argc,char* arg[])
{
	int n = -1; // 
	int c = 0;  //
	int w = 0;  //
	int i = 0;
	int* mark;   //  mark which arg has been used

	mark = (int*)malloc(argc * sizeof(int));
	for(i = 0; i < argc; i++)
	{
		mark[i] = 0;
	}
	//printf("in history\n");
	if(0 != findstring_from_arg(argc,arg,"history"))
	{
		write(STDERR_FILENO,"format error !! No call to the history command!!\r\n",50);
		return -1;
	}
	mark[0] = 1;

	if(-1 != (i = find_char_from_arg(argc,arg,'c')))
	{
		historyfile_clear();
		mark[i] = 1;
	}

	if(-1 != (i = findnum_from_arg(argc,arg)))
	{
		n = atoi(arg[i]);
		mark[i] = 1;
		if(n >= history_ser_number) //if n> number of all histroy , print all
		{
			history_print(1,history_ser_number - 1);
		}
		else 
		{
			history_print(history_ser_number - n,history_ser_number - 1);
		}
	}
	else
	{
		history_print(1,history_ser_number - 1);
	}

	if(-1 != (i = find_char_from_arg(argc,arg,'w')))
	{
		int beginnum = 0;
		mark[i] = 1;
		write_history(history_file_fd,history_buf_mark,history_i - 1);
		beginnum = history_i > FILEHEAD_SIZE ? history_i - FILEHEAD_SIZE : 0;
		if(history_i > FILEHEAD_SIZE)
		{
			history_moveto_head(beginnum,history_i - 1);
		}
	}

	// error judge

	//
	Nooptions(printf_fd, "history", mark,arg);
	return 0;
}


int inti_history_env()
{

}

int get_hissymbol(char* command_content, char* string) 
{
	int slen = 0;
	int i = 0;
	int j = 0;
	if(NULL == command_content || NULL == string)
	{
		return -1;
	}
	//
	i = 0;
	while('!' != command_content[i])
	{
		i++;
	}
	//
	if('!' == command_content[i + 1] || '$' == command_content[i + 1] 
		|| '^' == command_content[i + 1] )  // !!  !$  !^
 	{
		for(j = 0; j < 2; j++,i++)
		{
			string[j] = command_content[i];
		}
		string[j] = '\0';
		slen = j;
	}
	else if(isdigit(command_content[i + 1]))  //!N
	{
		j = 0;
		string[j] = '!';
		i++;
		j++;
		for(; isdigit(command_content[i]); j++,i++)
		{
			string[j] = command_content[i];
		}
		string[j] = '\0';
		slen = j;
	}
	else if('-' == command_content[i + 1]) // !-N
	{
		j = 0;
		string[j++] = command_content[i++];
		string[j++] = command_content[i++];
		for (; isdigit(command_content[i]); i++, j++)
		{
			string[j] = command_content[i];
		}
		string[j] = '\0';
		slen = j;
	}
	else if('_' == command_content[i + 1] || isalpha(command_content[i + 1]))  // !string
	{
		j = 0;
		string[j] = '!';
		i++;
		j++;
		for(;isalnum(command_content[i]) || '_' == command_content[i];i++,j++)
		{
			string[j] = command_content[i];
		}
		string[i] = '\0';
		slen = j;
	}
	

	return slen;
}

int analysis_hissymbol(char* string,char* sret)  // symbol's value get from calculated
 // match failed return -1 ,success return 0
{
	int i = 0;

	if(NULL == string || NULL == sret )
	{
		return -1;
	}
	if(string[0] != '!')
	{
		return -1;
	}
	// !!                 重新执行上一条命令
	if(string[1] == '!')
	{
		if( history_i != 0)
		{
			strcpy(sret,history_table.history_info[history_i - 2].command);
			return 0;
		}
		return -1;
	}
    //!N               重新执行第N条命令
    if(isdigit(string[1]))
    {
    	int digit = atoi(string + 1);
    	if(digit != 0 && digit < history_i)
    	{
    		strcpy(sret,history_table.history_info[digit - 1].command);
    		return 0;
    	}
    	return -1;
    }
    //!-N              重新执行倒数第N条命令
    if('-' == string[1])
    {
    	int digit = atoi(string + 2);
    	digit = -digit;
    	if(digit != 0 && digit < history_i)
    	{
    		strcpy(sret,history_table.history_info[digit - 2].command);
    		return 0;
    	}
    	return -1;
    }

    //!string        重新执行以字符串开头的命令
    if(isalpha(string[1]))
    {
    	int j = 0;
    	int no =0;
    	string = string + 1;
    	for(i = 0; i < history_i; i++)
    	{
    		no = 0;
    		j = 0;
    		while(string[j] == history_table.history_info[i].command[j])
    		{
    			if(string[j] == '\0' && string[j] == ' ')
    			{
    				strcpy(sret,history_table.history_info[i].command);
    				return 0;
    			}
    		}
    	}
    	return -1;
    }
    //!?string?   重新执行包含字符串的命令
    //!$               上一条命令的最后一个参数
    if('$' == string[1])
    {
    	if(history_i > 0)
    	{
    		for(i = strlen(history_table.history_info[history_i - 2].command); 
    			i >= 0 && ' ' != history_table.history_info[history_i - 2].command[i] ;i--)
    		{

    		}
    		strcpy(sret,history_table.history_info[history_i - 2].command + i + 1);
    		return 0;
    	}
    	return -1;
    }
    //!^               上一条命令的第一个参数
    if('^' == string[1])
    {
    	if(history_i > 0)
    	{
    		for(i = 0; history_table.history_info[history_i - 2].command[i] != '\0'; i++)
    		{
    			if(' ' == history_table.history_info[history_i - 2].command[i])
    			{
    				sret[i] = '\0';
    				return 0;
    			}
    			sret[i] = history_table.history_info[history_i - 2].command[i];
    		}
    		sret[i] = '\0';
    		return 0;
    	}
    	return -1;
    }
    //!cmd:n     上一条命令的第n个参数
    //if()
    //!*              上一条命令的所有参数
    /*!*:x            x表示修饰符
        :s/old/new/ 对第N条指令中第一次出现的new替换为old
        :gs/old/new/ 全部替换
        :n 取指令的第w个参数
        :p 回显命令而不是执行
        :h 选取路径部分dirname
        :t 选取文件名部分basename
        :r 选取文件名filename
        :e 选取扩展名*/
}

int history_serch()
{

}



