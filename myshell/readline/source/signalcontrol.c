#include "signalcontrol.h"

int sonwait = 0;
int fatherwait = 0;
int wait_spid = 0;

int signal_Adaptation()
{
	signal(SIGWINCH,signal_sigwinch);
	signal(SIGINT,signal_sigint);
	signal(SIGTSTP,signal_sigtstp);
	signal(SIGQUIT,signal_sigquit);
	signal(SIGUSR1,signal_sigusr1);
	signal(SIGCHLD,signal_sigchld);
	return 0;
}

void signal_sigwinch(int sig)
{
	//write(STDOUT_FILENO,"\nget signal : sigwinch\n",23);
	ioctl(STDIN_FILENO,TIOCGWINSZ,&windows_info);
	setsrceen_from_size_cursor(nowcommand_point);
	disline_realleng = strlen(disline) % windows_info.ws_col;
	return ;
}

void signal_sigint(int sig)  // ^C
{

	//write(STDOUT_FILENO,"\nget signal : sigint\n",21);
	return ;
}

void signal_sigtstp(int sig)   // ^Z   ^Y
{
	int i = 0;
	int state = 0;
	/*for(i = 0; i < process_table.end ; i++)
	{
		waitpid(process_table.process_info[i].pid, &state, WNOHANG);
		if(WIFSTOPPED(state))
		{
			change_process_state(process_table.process_info[i].pid,state);
		}
	}*/
	//write(STDOUT_FILENO,"\nget signal : sigtstp\n",22);
	return ;
}

void signal_sigquit(int sig)  // "^\"
{
	//write(STDOUT_FILENO,"\nget signal : sigquit\n",22);
	return ;
}

void signal_sigchld(int sig)
{
	int i = 0;
	int state = 0;
	int returnpid = 0;
	int dis = 0;
	//write(STDOUT_FILENO,"\nget signal : sigchld\n",22);
	for(i = 0; i < process_table.end; i++)
	{
		/*if(wait_spid == process_table.process_info[i].pid)
		{
			continue ;
		}*/
		returnpid = waitpid(process_table.process_info[i].pid, &state, WNOHANG | WUNTRACED | WCONTINUED);
		if(0 == returnpid)
		{
			continue ;
		}
		if(WIFSTOPPED(state))
		{
			dis = 1;
			if(1 == on_read)
			{
				printf("\n");
			}
			printf("%d stop\n",process_table.process_info[i].pid);
			if(1 == on_read)
			{
				print_nowscreen();
			}
			change_process_state(process_table.process_info[i].pid,state);
		}
		if(WIFEXITED(state))
		{
			dis = 1;
			if(1 == on_read)
			{
				printf("\n");
			}
			if(2 == process_table.process_info[i].state)
			{
				printf("%d exit\n",process_table.process_info[i].pid);
			}
			if(1 == on_read)
			{
				print_nowscreen();
			}
			delete_proinfo_pid(process_table.process_info[i].pid);
		}
		if(WIFSIGNALED(state))
		{
			dis = 1;
			printf("%d Abnormal termination\n",process_table.process_info[i].pid);
		}
		if(WIFCONTINUED(state))
		{
			dis = 1;
			if(1 == on_read)
			{
				printf("\n");
			}
			printf("%d continue\n",process_table.process_info[i].pid);
			if(1 == on_read)
			{
				print_nowscreen();
			}
			change_process_state(process_table.process_info[i].pid,2);
			/*if(1 == bg_lock)
			{
				bg_lock = 0;
			}
			havesignal = 1;*/
		}
	}
	/*pid = waitpid(-1,&state,WNOHANG | WUNTRACED );
	if(WIFSTOPPED(state))
		{
			printf("%d stop\n",pid);
			//change_process_state(process_table.process_info[i].pid,state);
		}
		if(WIFEXITED(state))
		{
			printf("%d exit\n",pid);
			//delete_proinfo_pid(process_table.process_info[i].pid);
		}
		if(WIFSIGNALED(state))
		{
			printf("%d Abnormal termination\n",pid);
		}*/
	return ;
}

void signal_sigusr1(int sig)
{
	//write(STDOUT_FILENO,"\nget signal : sigusr1\n",22);
	sonwait = 0;
	return ;
}