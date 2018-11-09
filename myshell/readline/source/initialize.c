#include "initialize.h"

// get env and set terminal


int initialize()
{
	//bulid_session();
	signal_Adaptation();
	getwin_info();   // Getting terminal window information
	getuserinfo();
	getcdir();     //get directory
	get_homedir();
	set_workdir();
	inti_history();
	inti_pentv(&msh_envtable);
	inti_alias();
	//inti_sysenv();
	intianalysis();
	inti_processtable();

	//print_histable(0,history_table.size - 1);
	datastuct_init();  //  Data structure initialization
	intimode();     // Setting up terminal window mode
	open_all_switch();
	
	return 0;
}

int bulid_session()
{
	return setsid();
}

int inti_history()
{
	inithisfiledir();
	initialize_history_table();
	inti_history_env();
	//printf("print history_table\n");
	//print_histable(0,511);
	return 0;
}



int quitclear()
{
	free_processtable();
	close_history();
	free_command_path();
	back_mode();
	clear_env(&msh_envtable);
	close_alias();
	return 0;
}

int getuserinfo()
{
	uid = getuid();
	user_inf = getpwuid(uid);
	return 0;
}

int getcdir()   // Get the current directory
{
	
	struct stat rest;
	int i = 0;
	int j = 0;
	// get user inf
	

	for(i = 0;user_inf->pw_dir[i] != 0;i++)
	{
		begindir[i] = user_inf->pw_dir[i];
		nowdir[i] = user_inf->pw_dir[i];
	}

	begindir[i] = 0;
	nowdir[i] = 0;
	//
	//Establish directory existence
	if(-1 == stat(nowdir,&rest))
	{
		printf("The user directory does not exist !\n");
		mkdir(nowdir,S_IRWXU | S_IROTH | S_IXOTH);
	}
	//set env********
	// set   ~
	for(i=0; user_inf->pw_dir[i] != 0; i++)
	{
		user_bgdir.value[i] = user_inf->pw_dir[i];
	}
	user_bgdir.value[i] = 0;
	user_bgdir.name[0] = '~';
	user_bgdir.name[1] = 0;
	// end ~

	//************
	// initizlize disline
	strcpy(disline,disline_normal);
	for(i = 8,j = 0; user_inf->pw_name[j] != 0; i++, j++)
	{
		disline[i] = user_inf->pw_name[j];
	}
	disline[i] = ':';
	i++;
	disline_dirloc = i;
	// assignment disline_normal
	disline[i] ='\0';
	strcpy(disline_normal,disline);
	//printf("inti : disline_dirloc : %d\n",disline_dirloc);
	/*
	for(j = 0; nowdir[j] != 0; i++, j++)
	{
		disline[i] = nowdir[j];
	}
	*/
	disline[i] = '~';
	i++;
	disline[i] = '$';
	i++;
	disline[i] = ' ';
	i++;
	disline[i] = 0;
	//
	//printf("inti disline : %s\n",disline);

	return 0;
}

int datastuct_init()
{
	msh_envtable.number = 0;
	msh_envtable.ptable = NULL;

	// command stack
	stack_command.sp = 0;
	stack_char.sp = 0;
	//
}

int getwin_info()
{
	ioctl(STDIN_FILENO,TIOCGWINSZ,&windows_info);
	return 0; 
}

int get_homedir()
{
	dir_assignment(&home,user_inf->pw_dir);
	//printf("home : %s\n",user_inf->pw_dir );
	if(-1 == access(home.directory,F_OK))
	{
		write(STDIN_FILENO,"\r\nNO home directory !\r\n",23);
		if(-1 == mkdir(home.directory,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH))
		{
			write(STDERR_FILENO,"mkdir : \"user home\" failed ",26);
			return -1;
		}
	}
	return 0;
}

int set_workdir()
{
	getcwd(programdir,DIRECTORY_SIZE);
	strcat(programdir,"/myshell");
	//printf("programdir: %s",programdir);
	dir_copy(&workdir,&home);
	chdir(workdir.directory);
	//printf("workdir : %s\r\n",workdir.directory);
	return 0;
}

