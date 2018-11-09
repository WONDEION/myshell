#include "analysis.h"

int develop_his_switch = 0;
int develop_env_switch = 0;
int develop_cal_switch = 0;
int develop_ali_switch = 0;
int develop_que_switch = 0;

int intianalysis()
{
	return open_all_switch();
}

int close_all_switch()
{
	develop_his_switch = 1;
	develop_env_switch = 1;
	develop_cal_switch = 1;
	develop_ali_switch = 1;
	develop_que_switch = 1;
	return 0;
}

int open_all_switch()
{
	develop_his_switch = 0;
	develop_env_switch = 0;
	develop_cal_switch = 0;
	develop_ali_switch = 0;
	develop_que_switch = 0;
	return 0;
}

int analysis_command_primary(char* command)
{
	//command_type temp_command;
	int state;
	// develop history
	//printf("command : %s\n", command);
	if(0 == develop_his_switch)
	{
		develop_history(command);
	}
	//after develop_history ,we need processinput again
	if('\0' == command[0])
	{
		return 0;
	}
	if( -1 == process_input(command))
	{
		return 0;
	}
	if(0 == develop_env_switch)
	{
		if(-1 == env_develop(command))
		{
			return 0;
		}
	}
	//printf("here\n");
	if(0 == develop_ali_switch)
	{
		develop_alias(command);
	}
	//展开~变量  
	command_develop(command);
	//printf("analysis times\n")
	// develop calcultion
	if(0 == develop_cal_switch)
	{
		devecalculation(command);
	}
	// Removal of reference
	if(iscommandgroup(command))
	{
		state = gotocommandgroup(command);
		return state;
	}
	// 是否是组命令 队列 
	else if(isqueuecommand(command))
	{
		state = gotoqueuecommand(command);
	}
	// 是否是重定向
	else if(isredirect(command))
	{
		state = gotoredirect(command);
	}
	// 命令可以执行一条单独的表达式计算
	else 
	{
		
		state = analysis_singlecommand(command);
	}
	// set env
	return state;
}

int command_develop(char* command)
{
	int i = 0;
	if(NULL == command)
	{
		return -1;
	}
	// develop directory "~"
	develop_directory(command);
	return 0;
}

int develop_directory(char* command)
{
	int i = 0;
	int j = 0;
	int dirleng = 0;

	if(NULL == command)
	{
		return -1;
	}

	for(i = 0; '\0' != command[i]; i++)
	{
		if('\'' == command[i] )   // in  '' donot develop
		{
			i++;  // step
			while('\'' != command[i] && command[i] != '\0')
			{
				i++;
			}
		}
		else 
		{
			if( '~' == command[i] && (0 == i || ' ' == command[i - 1] || '\n' == command[i - 1]) )
			{
				//printf("in develop\n");
				dirleng = strlen(home.directory);
				//printf("\n dirleng : %d\n",dirleng);
				string_translation(command + i, 1, dirleng - 1);
				//printf("\ncommand : %s|\n", command);
				//printf("home : %s | i : %d\n", home.directory,i);
				for(j = 0; j < dirleng; j++,i++)
				{
					command[i] = home.directory[j];
				}
				//printf("command : %s|\n", command);
				i--;
				//printf("out develop\n");
			}
		}

	}
	return 0;
}

int develop_history(char* command)
{
	int i = 0;
	int j = 0;
	char buf[MAXCOMMAND_SIZE];
	char hisbuf[MAXSYMBOL_SIZE];
	int symbolleng = 0;
	int bufleng = 0;
	int stepleng = 0;

	if(NULL == command)
	{
		return -1;
	}
	for (i = 0; '\0' != command[i]; ++i)
	{
		if('\'' == command[i] )   // in  '' donot develop
		{
			i++;  // step '\''
			while('\'' != command[i] && command[i] != '\0')
			{
				i++;
			}
		}
		else if('!' == command[i] && ' ' != command[i+1])
		{
			if(i > 0)
			{
				if(' ' != command[i - 1] && ';' != command[i - 1] && '|' != command[i - 1] && '&' != command[i - 1] && ',' != command[i - 1])
				{
					continue ;
				}
			}
			symbolleng = get_hissymbol(command + i, hisbuf);
			//printf("hisbuf : %s\n", hisbuf);
			if(symbolleng >= 0)
			{
				if(-1 == analysis_hissymbol(hisbuf,buf))
				{
					continue ;
				}
				//printf("buf : %s\n", buf);
				bufleng = strlen(buf);
				stepleng = bufleng - symbolleng;
				string_translation(command + i + symbolleng, 0, stepleng);
				for (j = 0; j < bufleng ; ++j , i++)
				{
					command[i] = buf[j];
					/* code */
				}
				// 小心
				i--;
			}
		}
	}

	return 0;
}

int remove_quote(char* command)
{
	int i = 0;
	int j = 0;
	char ech[32];
	int ech_leng = 0;

	if(NULL == command)
	{
		return -1;
	}
	//write(STDOUT_FILENO,"in remove_quote\n",16);
	for(i = 0 ; command[i] != '\0';i++)
	{
		if( '\'' == command[i])
		{
			if('$' == command[i - 1])
			{
				string_translation(command, i + 1, -2); 
				i--;
				// remove ''' '$'
				while( '\'' != command[i] && command[i] != '\0')
				{
					// if Escape character
					if('\\' == command[i]) 
					{
						ech_leng = get_escape_character(command + i, ech);
						//msh_fprintf(STDOUT_FILENO,"ech_leng : %d\n",ech_leng);
						if(ech_leng != 0)  // is escape character
						{
							string_translation(command + i, ech_leng , -(ech_leng - 1));
							command[i] = ech[0];
						}
					}
					i++;
				}
				if('\'' == command[i])
				{
					string_translation(command, i + 1, -1);
					i--;
				}
			}
			else  // do nothing
			{
				string_translation(command,i + 1,-1);
				while( '\'' != command[i] && command[i] != '\0')
				{
					i++;
				}
				if('\'' == command[i])
				{
					string_translation(command, i + 1, -1);
					i--;
				}
			}
		}
		if( '\"' == command[i])
		{
			char dou_tch = 0;
			string_translation(command,i + 1, -1);
			while('\"' != command[i] && command[i] != '\0')
			{
				if('\'' == command[i])
				{
					remove_quote(command + i);
				}
				if('\\' == command[i])
				{
					switch(command[i + 1])
					{
						case '\\':	dou_tch = '\\';
									break;
						case 'r':	dou_tch = 13;
									break;
						case 'n':	dou_tch = 10;
									break;
						default:	break;
					}
					if(dou_tch != 0)
					{
						command[i] =  dou_tch;
						string_translation(command + i, 2 , -1);
					}
				}
				i++;
			}
			if('\"' == command[i])
			{
				string_translation(command,i + 1, -1);
			}
		}
	}
}

int get_escape_character(char* string, char* pch)  // return number of Escape character
{
	int i = 0;
	int j = 0;
	int k = 0;
	int digit = 0;
	int number = 0;

	if(NULL == string || NULL == pch)
	{
		return -1;
	}
	for(i = 0 , j = 0; string[i] != '\0'; i++)
	{
		if('\\' == string[i])
		{
			number += 1;
			switch(string[i + 1])
			{
				case 'a':	pch[j++] = 7;
							number += 1;
							break;
				case 'b':	pch[j++] = 8;
							number += 1;
							break;
				//case 'e':	pch[j++] = ;
				//			break;
				case 'f':	pch[j++] = 12;
							number += 1;
							break;
				case 'n':	pch[j++] = 10;
							number += 1;
							break;
				case 'r':	pch[j++] = 13;
							number += 1;
							break;
				case 't':	pch[j++] = 9;
							number += 1;
							break;
				case 'v':	pch[j++] = 11;
							number += 1;
							break;
				case '\\':	pch[j++] = 92;
							number += 1;
							break;
				case '\'':	pch[j++] = 39;
							number += 1;
							break;
				case 'c' :	pch[j++] = string[i + 2] >= 97 ? string[i + 2] - 'a' + 1 : string[i + 2] - 'A' + 1;
							number += 1;
							break;
				default :	if('x' == string[i + 1])
							{
								int temp_digit = 0;
								for(k = i + 1;0 != (temp_digit = issthex(string[k])) && k < i + 4; k++)
								{
									number += 1;
									 digit = digit * 16 + temp_digit;
								}
								//
								pch[j++] = digit;
							}
							else if(isdigit(string[k]))
							{
								for(k = i + 1; isdigit(string[k]) && k < i + 4; k++)
								{
									number += 1;
									digit = digit * 8 + string[k] - '0'; 
								}
								//
								pch[j++] = digit;
							}
							else
							{
								number = 0;
							}
							break ;
			}
			pch[j] ='\0';
			break ;
		}
	}
	return number;
}


int analysis_singlecommand(char* command)
{
	int state = 0;
	//write(STDOUT_FILENO,"single command!\n",16);
	if( NULL == command )
	{
		return -1;
	}
	if(isbuildenv(command))
	{
		if(0 == develop_que_switch)
		{
			remove_quote(command);
		}
		//printf("isbuildenv\n");
		state = buildenv_command(&msh_envtable,command);
		//printenvtable(&msh_envtable);
		return state;
	}
	if(isbarcecommand(command))
	{
		if(0 == develop_que_switch)
		{
			remove_quote(command);
		}
		state = gotobracketscommand(command);
		return state;
	}
	// 
	if(0 == develop_que_switch)
	{
		remove_quote(command);
	}
	if(-1 != isshellcommand(command))
	{
		//printf("shell command\n");
		state = gotoshellcommand(command);
		return state;
	}
	if(islinuxcommand(command))
	{
		state = execute_linuxcommand(command);
		return state;
	}
	if(isexecfile(command))
	{
		state = execfile(command);
		return state;
	}
	write(STDOUT_FILENO,"no command : \"",14);
	write(STDOUT_FILENO,command,strlen(command));
	write(STDOUT_FILENO,"\" !!\n",5);
	return state;
}



int process_input(char* command)
{
	if(NULL == command)
	{
		return -1;
	}
	// judege brace match
	if(1 != is_brace_matching(command))
	{
		write(STDOUT_FILENO,"The parentheses of the command do not match\n",44);
		return -1;
	}
	// quote
	if(1 != is_quotes_match(command))
	{
		write(STDOUT_FILENO,"The quotes of the command do not match\n",40);
		return -1;
	}
	//Clear the superfluous space
	clear_superfluous_space(command);
	return 0;
}

int clear_superfluous_space(char* string)
{
	char bufcommand[MAXCOMMAND_SIZE];
	int i = 0;
	int j = 0;
	if(NULL == string)
	{
		return -1;
	}
	tab_to_space(string);
	// remove head
	i = 0;
	while(' ' == string[i])
	{
		i++;
	}
	// remove tail
	j = strlen(string);
	while(' ' == string[j - 1])
	{
		string[j - 1] = string[j];
		j--;
	}
	//
	for(j= 0; string[i] != '\0' ;i++,j++)
	{
		if(' ' == string[i])
		{
			bufcommand[j] = string[i];
			while(' ' == string[i])
			{
				i++;
			}
			i--;
		}
		else
		{
			bufcommand[j] = string[i];
		}
	}
	bufcommand[j] = '\0';
	strcpy(string,bufcommand);

	return 0;
}

int tab_to_space(char* string)
{
	int i = 0;
	if(NULL == string)
	{
		return -1;
	}

	for( i = 0; string[i] != '\0'; i++)
	{
		if('\t' == string[i])
		{
			string[i] = ' ';
		}
	}
	return 0;
}

