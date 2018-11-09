
#ifndef MISSTAKE_H
#define MISSTAKE_H

// include
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//define

#define PRINTERROR(S) printf("\n\rProgram error!!!\n\r");printf(S);return -1

int shellcommandNOcall(int fd,char* name);
int Nodir_or_file(int fd,char* dir);
int Nooptions(int fd, char* commandname, int mark[], char** arg);

#endif