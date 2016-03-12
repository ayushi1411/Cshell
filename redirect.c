#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "header.h"
extern int redirect;
int check_redirect(char * pipe_command)
{
	int i=0;
	char * token;
	char * tempcommand=strdup(pipe_command);
	token=strtok(tempcommand," <>\t\a\r");
	in=NULL;
	out=NULL;
	redirect=0;
	while(token!=NULL)
	{
		if(!redirect)
		{
			argv[i]=strdup(token);
			i++;
		}

		int pos=token-tempcommand+strlen(token);
		char ch1=pipe_command[pos];
		char ch2=pipe_command[pos+1];
		token=strtok(NULL," <>\t\a\r");
		if(ch1=='<' || ch2=='<')
		{
			in=token;
			redirect=1;
		}
		if(ch1=='>' || ch2=='>')
		{
			out=token;
			redirect=1;
		}

	}
	argv[i]=NULL;
	return redirect;
}
