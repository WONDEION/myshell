#include "press_tab.h"

int handle_tab(command_type* pcommand)
{
	static int times = 0;
	static char type = 0;
	static char word[MAXCOMMAND_SIZE];
	static char save[MAXCOMMAND_SIZE];
	int i = 0;
	int state = 0;
	int local = 0;
	if(NULL == pcommand)
	{
		return -1;
	}
	if( -1 == (i = get_word(pcommand,word)))
	{
		return -1;
	}
	if(1 == times)
	{
		if(strcmp(word,save))
		{
			times = 0;
		}
	}
	if(0 == times)
	{
		times = 1;
		//printf("\nget_word : %s | i : %d\n",word,i);
		type = word_type(word,i);
		local = strlen(word);
		//printf("\nlocal : %d | type : %c\n",local,type);
		if('c' == type)
		{
			state = find_command_and_develop(word);
		}
		else if('d' == type)
		{
			state = find_file_and_develop(word,1);
		}
		//printf("\nstate : %d\n",state);
		if(1 == state)
		{
			times = 0;
			// respace
			for( i = local; '\0' != word[i]; i++)
			{
				add_and_print(pcommand,word[i]);
			}
		}
		else if(0 == state)  // haven't
		{
			times = 0;
		}
		else if(2 == state)
		{
			strcpy(save,word);
			//do nothing
		}
	}
	else if(1 == times)
	{
		//printf("\nin times == 1\n");
		times = 0;
		if('c' == type)
		{
			print_allcommand(pcommand,word);	
		}
		else if('d' == type)
		{
			print_allfile(pcommand,word);
		}
	}
	return 0;
}

int get_word(command_type* pcommand,char* word) // -1 no word  
{
	int i = 0;
	int type = 0;  // 0 command 1 parameter
	int local = 0;
	if(NULL == pcommand || NULL == word)
	{
		return -1;
	}
	for(i = pcommand->cursorlocal - 1; i >= 0 ; i--)
	{
		if(' ' == pcommand->command_content[i] 
			|| ';' == pcommand->command_content[i])
		{
			break;
		}
	}
	if(i < 0 || ';' == pcommand->command_content[i]) // it is command
	{
		local = i + 1;
		type = 0;
	}
	else  // is ' '
	{
		local = i + 1;
		while(' ' == pcommand->command_content[i] && i <= 0)
		{
			i--;
		}
		if(i < 0 || ';' == pcommand->command_content[i]) // it is command
		{
			type = 0;
		}
		else
		{
			type = 1;
		}
	}
	// copy;
	for (i = 0; local < pcommand->cursorlocal; ++i , local++)
	{
		word[i] = pcommand->command_content[local];
		/* code */
	}
	word[i] = '\0';
	return type;
}

int word_type(char* word,int i)
{
	if(NULL == word)
	{
		return -1;
	}
	if(0 == i)
	{
		if('.' == word[0] || '/' == word[0])   // exec file
		{
			return 'd';
		}
		else
		{
			return 'c';
		}
	}
	else if(1 == i)
	{
		return 'd';
	}
	return 0;
}

int find_command_and_develop(char* word)
{
	int i = 0;
	int times = 0;
	char save[MAXCOMMAND_SIZE];
	if(NULL == word)
	{
		return -1;
	}
	for(i = 0 ; shellcommand_nametable[i] != NULL ;i++)
	{
		if(tab_match_string(shellcommand_nametable[i],word))
		{
			times++;
			if(times == 1)
			{
				strcpy(save,shellcommand_nametable[i]);
			}
		}
	}
	if(1 == times)
	{
		strcpy(word,save);
		return 1;
	}
	else if(times > 1)
	{
		return 2;
	}
	// linux command;
}

int find_file_and_develop(char* word,int mode)  //Put in word
// mode 1 is 
// mode 2 is print all 
{
	char buf[DIRECTORY_SIZE];
	int i = 0;
	int state;
	int local = 0;
	if(NULL == word)
	{
		return -1;
	}
	//printf("\nword : %s\n", word);
	if('/' == word[0])
	{
		strcpy(buf,word);
		delete_lastlv_dir_string(buf);
		//get match word local
		local = get_match_word_local(word);
	}
	else if(('.' == word[0] && '/' == word[1]) 
			|| ('.' == word[0] && '.' == word[1] && '/' == word[2]))
	{
		if('.' == word[0] && '/' == word[1])
		{
			strcpy(buf,nowdir);
			strcat(buf,word + 1);
			delete_lastlv_dir_string(buf);
			local = get_match_word_local(word);
		}
		else
		{
			strcpy(buf,nowdir);
			delete_lastlv_dir_string(buf);
			strcat(buf,word + 2);
			delete_lastlv_dir_string(buf);
			local = get_match_word_local(word);
		}
	}
	else
	{
		strcpy(buf,nowdir);
		local = get_match_word_local(word);
		//printf("\nnowdir : %s\n", buf);
	}
	//match
	//printf("\nbuf : %s | word + local : %s\n", buf,word + local);
	state = match_in_dir(buf,word + local,mode);

	return state;

}

int get_match_word_local(char const* word)
{
	int i = 0;
	if(NULL == word)
	{
		return -1;
	}
	for(i = strlen(word) - 1; i >= 0 ; i--)
	{
		if('/' == word[i])
		{
			break;
		}
	}
	i++;
	return i;
}

int match_in_dir(char* dir,char* word,int mode)  // Put in word
{
	DIR *dp = NULL;
	struct dirent *dirp = NULL;
	int times = 0;
	char savefile[NAME_MAX];
	if(NULL == dir|| NULL == word)
	{
		return -1;
	}

	dp = opendir(dir);
	if(NULL == dp)
	{
		printf("match_in_dir : opendir error !!\n");
		return -1;
	}
	if( 2 == mode)
	{
		write(STDOUT_FILENO,"\nThese file begin with \"",24);
		write(STDOUT_FILENO,word,strlen(word));
		write(STDOUT_FILENO,"\"\n",2);
	}
	//printf("\n dir : %s\n", dir);
	while(NULL != (dirp = readdir(dp)))
	{
		//printf("\nin loop\n");
		if(tab_match_string(dirp->d_name,word))
		{
			//printf("\nmatch success\n");
			if(1 == mode)
			{
				times++;
				if(1 == times)
				{
					strcpy(savefile,dirp->d_name);
				}
			}
			else if(2 == mode)
			{
				write(STDOUT_FILENO,dirp->d_name,strlen(dirp->d_name));
				write(STDOUT_FILENO,"\n",1);
			}
		}
	}
	if(2 == mode)
	{
		return 0;
	}
	if(times > 1)
	{
		return 2;
	}
	else if(1 == times)
	{
		strcpy(word,savefile);
		return 1;
	}
	return 0;
}

int tab_match_string(char const* source,char const* object)
{
	int i = 0;
	if(NULL == source || NULL == object)
	{
		return 0;
	}
	//printf("\nsource : %s | object : %s\n",source,object);
	while(source[i] == object[i] && '\0' != source && '\0' != object)
	{
		i++;
	}
	if('\0' == object[i])
	{
		return 1;
	}
	return 0;
}

int print_allcommand(command_type* pcommand,char* word)
{
	int i = 0;
	if(NULL == word)
	{
		return -1;
	}
	write(STDOUT_FILENO,"\nAll commands beginning with \"",30);
	write(STDOUT_FILENO,word,strlen(word));
	write(STDOUT_FILENO,"\"\n",2);
	for(i = 0; shellcommand_nametable[i] != NULL ; i++)
	{
		if(tab_match_string(shellcommand_nametable[i],word))
		{
			write(STDOUT_FILENO,"  ",2);
			write(STDOUT_FILENO,shellcommand_nametable[i],
					strlen(shellcommand_nametable[i]));
			write(STDOUT_FILENO,"\n",1);
		}
	}
	display_linehead();
	printf_command_withcursor(pcommand);
	return 0;
}	

int print_allfile(command_type* pcommand,char* word)
{
	if(NULL == pcommand || NULL == word)
	{
		return -1;
	}
	find_file_and_develop(word,2);
	display_linehead();
	printf_command_withcursor(pcommand);
	return 0;
}