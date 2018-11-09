
#ifndef PROCSEE_H
#define PROCESS_H

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h> 
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "Datastruct.h"
#include "selflib.h"
#include "signalcontrol.h"


//env
int applyspace_envtable(envtable* penvt);
int env_copy(environment_variable* target,environment_variable* source);  // NO malloc
int addto_envtable(envtable* penvt, environment_variable* envariable);
int addto_envtable_namevalue(envtable* penvt, char* name, char* value);
int isbuildenv(char* command);
int build_env(environment_variable* penvt, char* string);
int buildenv_command(envtable* pentable, char* string);
int buildenv_charvalue(envtable* pentable, char* name, char* value);
int ass_env(environment_variable* penv, char* key, char* value);
int deletenv(envtable* penvt,environment_variable* env);
int deletenv_name(envtable* penvt,char* name);
int updatesysenv(char* name,char* value);
int updatemshenv(char* name,char* value);
int updateallenv(char* name,char* value);
int inti_pentv(envtable* penvt);
int inti_sysenv();
int get_from_msh_env(char* name, char* value); // -1 means failed
int get_from_sys_env(char* name, char* value); // -1 means failed
int get_from_all_env(char* name, char* value); // -1 means failed
int get_from_envtable(envtable* pentable,char* name,char* value); // -1 means failed
int env_develop(char* string);
int isenvnamechar(char ch);
int clear_env(envtable* penvt);
int printenvtable(envtable* penvt);
//process
//int setgroup(pid_t s_pid,int wait);
int inti_processtable();
int free_processtable();
int clear_processtable();
int clear_processinfo(process_info_type* proinfo);
int find_process_pid(int pid);
int processinfo_copy(process_info_type* pdest, process_info_type* psource);
int delete_proinfo(int i);
int delete_proinfo_pid(int pid);
int addto_processtable(process_info_type* pinfo);
int addto_processtable_dispersed(char* name,pid_t pid,int state,char** arg);
int change_process_state(int pid,int state);

extern char** environ;

extern process_table_type process_table;

extern pid_t session_id;
extern pid_t fount_gid;
extern pid_t back_gid;


#endif