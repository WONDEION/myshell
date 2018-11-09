#include "process.h"

process_table_type process_table;

pid_t session_id = -1; 
pid_t fount_gid = -1;
pid_t back_gid = -1;

int applyspace_envtable(envtable* penvt)
{
	if(NULL == penvt)
	{
		return -1;
	}
	if(NULL == penvt->ptable)
	{
		penvt->ptable = (environment_variable*)malloc(MALLOC_SIZE*sizeof(environment_variable*));
		penvt->size = MALLOC_SIZE;
		penvt->number = 0;
	}
	else
	{
		penvt->ptable = (environment_variable*)realloc(penvt->ptable,(penvt->size+MALLOC_SIZE)*sizeof(environment_variable*));
		penvt->size += MALLOC_SIZE;
	}
	return 0;
}

int addto_envtable(envtable* penvt, environment_variable* envariable)
{
	int i = 0;
	char temp_value[MAXENV_SIZE];
	if(NULL == penvt)
	{
		return -1;
	}
	if(NULL == penvt->ptable)   //first 
	{
		applyspace_envtable(penvt);
	}
	if(penvt->number >= penvt->size)  // need to apply space
	{
		applyspace_envtable(penvt);
	}
	if(-1 == ( i = get_from_envtable(penvt,envariable->name,temp_value)))
	{
		i = penvt->number;
	}
	else
	{
		//printf("%d\n",i );
		printf("\"%s\" has been define !!\n", envariable->name);
	}
	// copy
	if(env_copy(&penvt->ptable[penvt->number],envariable))
	{
		return -1;
	}
	//
	penvt->number++;

	return 0;
}

int addto_envtable_namevalue(envtable* penvt, char* name, char* value)
{
	environment_variable temp_env;
	if(NULL == penvt || NULL == name || NULL == value)
	{
		return -1;
	}
	ass_env(&temp_env, name, value);
	return addto_envtable(penvt,&temp_env);
}

int env_copy(environment_variable* target,environment_variable* source)  // NO malloc
{
	int i = 0;

	if(source == NULL || target == NULL)
	{
		return -1;
	}
	// copy name
	for(i = 0; source->name[i] != 0; i++)
	{
		target->name[i] = source->name[i];
	}
	target->name[i] = 0;
	//value
	for(i = 0; source->value[i] != 0; i++)
	{
		target->value[i] = source->value[i];
	}
	target->value[i] = 0;
	//*********
	return 0;
}

int isbuildenv(char* command)
{
    int i = 0;
    char ch = 0;
    if(NULL == command)
    {
    	return 0;
    }
    for(i = 0; command[i] != '\0' && ' ' != command[i] ; i++)
    {
    	if('\'' == command[i] || '\"' == command[i])
    	{
    		ch = command[i];
    		i++;
    		while(ch != command[i])
    		{
    			i++;
    		}
    	}
    	else if('=' == command[i])
    	{
    		break;
    	}
    }
    if(0 == i || '\0' == command[i] || ' ' == command[i])
    {
    	return 0;
    }
    if(!isenvnamechar(command[i - 1]) || ' ' == command[i + 1])
    {
    	return 0;
    }
	return 1;
}

int buildenv_command(envtable* pentable, char* string)
{
	environment_variable temp_env;
	char temp_value[MAXENV_SIZE];
	if(NULL == pentable || NULL == string)
	{
		return -1;
	}
	if( -1 == build_env(&temp_env,string))
	{
		printf("build_env error\n");
		return -1;
	}
	if(-1 != get_from_envtable(pentable,temp_env.name,temp_value))
	{
		printf("Environment variable \"%s\" has been define!!\n", temp_env.name);
		return -1;
	}
	if( -1 == addto_envtable(pentable,&temp_env))
	{
		printf("addto_envtable error\n");
		return -1;
	}
	return 0;
}

int buildenv_charvalue(envtable* pentable, char* name, char* value)
{
	if(NULL == pentable || NULL == name || NULL == value)
	{
		return -1;
	}
	if(-1 != get_from_envtable(pentable,name,value))
	{
		printf("Environment variable \"%s\" has been define!!\n", name);
		return -1;
	}
	if( -1 == addto_envtable_namevalue(pentable,name,value))
	{
		printf("addto_envtable error\n");
		return -1;
	}
	return 0;
}

int build_env(environment_variable* penv, char* string)
{
	int i = 0;
	int j = 0;
	char buf_key[MAXENV_SIZE];
	char buf_value[MAXENV_SIZE];
	int who = 0;

	if(NULL == penv || NULL == string)
	{
		return -1;
	}
	for(i = 0 ; string[i] != '\0' && string[i] != ' ';i++)
	{
		if('=' == string[i])
		{
			who = 1;
			j = 0;	
			continue;
		}
		if(who == 0)
		{
			buf_key[j] = string[i];
			j++;
			buf_key[j] = '\0';
		}
		else
		{
			buf_value[j] = string[i];
			j++;
			buf_value[j] = '\0';
		}

	}
	if(who == 0)
	{
		return -1;
	}
	//printf("buf_key: \"%s\" buf_value \"%s\"\n", buf_key,buf_value);
	if(!ass_env(penv,buf_key,buf_value))
	{
		return 0;
	}
	else
	{
		//printf("build_env : ass_env error\n");
		return -1;
	}
}

int ass_env(environment_variable* penvt, char* key, char* value)
{
	int i = 0;
	if(NULL == penvt)
	{
		return -1;
	}
	for(i = 0; key[i] != '\0';i++)
	{
		penvt->name[i] = key[i];
	}
	penvt->name[i] = '\0';
	for(i = 0 ; value[i ] != '\0';i++)
	{
		penvt->value[i] = value[i];
	}
	penvt->value[i] = 0;

	return 0;
}

int addto_envtable_string(envtable* penvt,char* key, char* value)
{
	environment_variable temp_env;
	if( NULL == key || NULL == value || NULL == penvt)
	{
		return -1;
	}
	ass_env(&temp_env,key,value);
	addto_envtable(penvt,&temp_env);
	return 0;
}

int inti_sysenv()
{

}

int inti_pentv(envtable* penvt)
{
	environment_variable temp_env;
	directory_type temp_dir;
	penvt->size = 0;
	penvt->number = 0;
	penvt->ptable = NULL;

	return 0;
}

int get_from_msh_env(char* name, char* value)    // -1 means failed
{
	int i = 0;
	if(NULL == name || NULL == value)
	{
		return -1;
	}
	for(i = 0; i < msh_envtable.number ; i++)
	{
		if(0 == strcmp(name,msh_envtable.ptable[i].name))
		{
			strcpy(value,msh_envtable.ptable[i].value);
			return i;
		}
	}
	return -1;
}

int get_from_sys_env(char* name, char* value)  // -1 means failed
{
	char** choice = environ;
	char buf[MAXENV_SIZE];
	int i  = 0;
	int j = 0;

	if(NULL == name || NULL == value)
	{
		return -1;
	}

	for(; (*choice) != NULL; choice++)
	{
		for(i = 0 ,j = 0; (*choice)[i] != '=' ; i++, j++)
		{
			buf[i] = (*choice)[i];
		}
		buf[i] = '\0';
		if(0 == strcmp(buf,name))
		{
			strcpy(value,(*choice) + i + 1);
			return j;
		}
	}

	return -1;
}

int get_from_all_env(char* name, char* value)  // -1 means failed
{
	int i = 0;
	if(NULL == name || NULL == value)
	{
		return -1;
	}
	//printf("here\n");
	if(-1 != (i = get_from_msh_env(name,value)))
	{
		return i;
	}
	if(-1 != (i = get_from_sys_env(name,value)))
	{
		return i;
	}
	//**
	return -1;
}

int get_from_envtable(envtable* pentable,char* name,char* value) // -1 means failed
{
	int i = 0;
	if(NULL == pentable || NULL == name || NULL == value)
	{
		return -1;
	}
	if(0 == pentable->size || 0 == pentable->number)
	{
		return -1;
	}
	//printf("name : %s\n", name);
	//printenvtable(pentable);
	for( i = 0; i < pentable->number; i++)
	{
		//printf("name : %s | pentable->ptable[%d].name :\n",
			//	name,i, pentable->ptable[i].name);
		if(!strcmp(name,pentable->ptable[i].name))
		{
			strcpy(value,pentable->ptable[i].value);
			return i;
		}
	}
	return -1;
}


int deletenv(envtable* penvt, environment_variable* penv)
{
	return deletenv_name(penvt,penv->name);
}

int deletenv_name(envtable* penvt, char* name)
{
	int i = 0;
	if(NULL == penvt || NULL == name)
	{
		return -1;
	}
	for(i = 0; i < penvt->number ; i++)
	{
		if(!strcmp(penvt->ptable[i].name,name))
		{
			for( ; i < penvt->number; i++)
			{
				env_copy(penvt->ptable + i,penvt->ptable + i + 1);
			}
			penvt->number--;
			return 0;
		}
	}
	return -1;
}

int updatesysenv(char* name,char* value)
{
	int i = 0;
	int j = 0;
	char **choice = environ;
	if(NULL == name || NULL == value)
	{
		return -1;
	}

	for(; choice != NULL; choice++)
	{
		i = 0;
		j = 0;
		while(name[i] == (*choice)[i] && (*choice)[i] != '=')
		{
			i++;
		}
		if((*choice)[i] == '=')
		{
			i++;
			while(value[j] != '\0')
			{
				(*choice)[i] = value[j];
				i++;
				j++;
			}
			(*choice)[i] = '\0';
			return 0;
		}
	}
	return -1;
}

int updatemshenv(char* name,char* value)
{
	int i = 0;
	char temp_value[MAXENV_SIZE];
	if(NULL == name || NULL == value)
	{
		return -1;
	}
	i = get_from_msh_env(name,temp_value); // -1 means failed
	if(-1 == i)
	{
		return -1;
	}
	strcpy(msh_envtable.ptable[i].value,value);
	return i;
}

int updateallenv(char* name,char* value)
{
	int i = 0;
	if(NULL == name || NULL == value)
	{
		return -1;
	}
	i = updatemshenv(name,value);
	if(-1 != i)
	{
		return i;
	}
	else
	{
		i = updatesysenv(name,value);
		if(-1!= i)
		{
			return i;
		}
	}
	return -1;
}

int env_develop(char* string)
{
	int i = 0;
	int begin = 0;
	int end = 0;
	int j = 0;
	int nameleng = 0;
	int valueleng = 0;
	char value[MAXENV_SIZE];
	char name[MAXENV_SIZE];
	if(NULL == string)
	{
		return -1;
	}
	for(i = 0; string[i] != '\0'; i++)
	{
		if('\'' == string[i])
		{
			i++;
			while('\'' != string[i] && '\0' != string[i])
			{
				i++;
			}
		}
		else
		{
			if('$' == string[i])
			{
				begin = i;
				i++;
				for(j = 0; isenvnamechar(string[i]);j++,i++)
				{
					name[j] = string[i];
				}
				name[j] == '\0';
				end = i;
				i--;  // for loop i++ 
				// find env
				//printf("name : %s\n",name);
				if( -1 == get_from_all_env(name,value))
				{
					printf("no environment variable  : \"%s\"\n",name);
					return 1;
				}
				else  // replace
				{
					int k = 0;
					nameleng = strlen(name);
					valueleng = strlen(value);
					string_translation(string,end,valueleng - nameleng - 1);
					for( k = 0 ;k < valueleng; k++)
					{
						string[begin + k] = value[k];
					}
				}
			}	
		}
	}
	if(begin == 0 && end == strlen(string))
	{
		printf("myshell : %s\n",string);
		return 1;
	}
	return 0;
}

int isenvnamechar(char ch)
{
	if(isalnum(ch))
	{
		return 1;
	}
	if('_' == ch)
	{
		return 1;
	}
	return 0;
}

int clear_env(envtable* penvt)
{
	if(NULL == penvt)
	{
		return 0;
	}
	free(penvt->ptable);
	penvt->number = 0;
	penvt->size = 0;
	return 0;
}

int printenvtable(envtable* penvt)
{
	int i = 0;
	if(NULL  == penvt)
	{
		return -1;
	}
	if(NULL == penvt->ptable || 0 == penvt->size)
	{
		return -1;
	}
	for(i = 0; i < penvt->number ; i++)
	{
		printf("name : \"%s\"\n", penvt->ptable[i].name);
		printf("value : \"%s\"\n", penvt->ptable[i].value);
	}
	return 0;
}

//************************************************************************

int inti_processtable()
{
	process_table.process_info = (process_info_type*)
				malloc((RLIMIT_NPROC) * sizeof(process_info_type));
	process_table.mark = (int*)malloc((RLIMIT_NPROC) * sizeof(int));
	if(NULL == process_table.process_info || NULL == process_table.mark)
	{
		return -1;
	}
	return 0;
}

int free_processtable()
{
	free(process_table.process_info);
	free(process_table.mark);
	return 0;
}

int clear_processtable()
{
	int i = 0;
	for(i = 0; i < RLIMIT_NPROC; i++)
	{
		 process_table.mark[i] = 0;
		 process_table.end = 0;
		 clear_processinfo(process_table.process_info + i);

	}

	return 0;
}

int clear_processinfo(process_info_type* proinfo)
{
	if(NULL == proinfo)
	{
		return 0;
	}
	proinfo->pid = -1;
	proinfo->uid = -1;
	proinfo->name[0] = '\0';
	proinfo->state = -2;
	free_arg(proinfo->arg);
	proinfo->arg = NULL;

	return 0;
}


int find_process_pid(int pid)
{
	int i = 0;

	if(pid <= 0)
	{
		return -1;
	}
	for(i = 0; i < process_table.end ; i++)
	{
		if(pid == process_table.process_info[i].pid)
		{
			return i;
		}
	}
	return -1;
}


int processinfo_copy(process_info_type* pdest, process_info_type* psource)
{
	if(NULL == pdest || NULL == psource)
	{
		return -1;
	}
	pdest->pid = psource->pid;
	pdest->uid = psource->uid;
	strcpy(pdest->name, psource->name);
	pdest->state = psource->state;
	pdest->arg = psource->arg;

	return 0;
}

int delete_proinfo(int i)
{
	int j = 0;
	if(i < 0 || i >= process_table.end)
	{
		return -1;
	}
	clear_processinfo(process_table.process_info + i);
	process_table.mark[i] = 0;
	for(j = i; j < process_table.end ; j++)
	{
		processinfo_copy(process_table.process_info + j
			, process_table.process_info + j + 1);
	}
	process_table.end--;
	
	return 0;
}

int delete_proinfo_pid(int pid)
{
	int i = find_process_pid(pid);
	if(i == -1)
	{
		return -1;
	}
	return delete_proinfo(i);
}

int addto_processtable(process_info_type* pinfo)
{
	int i = 0;
	if(NULL == pinfo)
	{
		return -1;
	}

	i = process_table.end;

	if(i >= RLIMIT_NPROC)
	{
		write(STDERR_FILENO,"The number of processes exceeds the limit !!\n",45);
		return -1;
	}

	processinfo_copy(process_table.process_info + i,pinfo);
	process_table.mark[i] = 1;
	process_table.end++;

	return 0;
}

int addto_processtable_dispersed(char* name,pid_t pid,int state,char** arg)
{
	int i = 0;
	if(name == NULL)
	{
		return 0;
	}

	i = process_table.end;

	if(i >= RLIMIT_NPROC)
	{
		write(STDERR_FILENO,"The number of processes exceeds the limit !!\n",45);
		return -1;
	}

	process_table.process_info[i].pid = pid;
	process_table.process_info[i].state = state;
	strcpy(process_table.process_info[i].name,name);
		process_table.mark[i] = 1;
	process_table.process_info[i].arg = arg;
	process_table.end++;

	return 0;
}

int change_process_state(int pid,int state)
{
	int i = 0;
	if(!(1 == state || 2 == state || 0 == state || -1 == state))
	{
		return -1;
	}
	for(i = 0; i < process_table.end; i++)
	{
		if(pid == process_table.process_info[i].pid)
		{
			process_table.process_info[i].state = state;
			return 0;
		}
	}
	return 0;
}