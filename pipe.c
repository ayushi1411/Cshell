#include "header.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_PAR 10  //max number of parameters 
#define MAX_CMD_LENGTH 100 //max length of the command
extern int redirect;
void piping(char * pipe_command[],int count)
{
	int i=0;
	int fd[2];
	int pipe_in=0;
	pid_t pid;
	for(i=0;i<count;i++)
	{
                if(i!=0)
		      redirect=check_redirect(pipe_command[i]);
		int index=0;
		while(argv[index]!=NULL)
		{
			index++;
		}
		if(strcmp(argv[index-1],"&")==0)//if the last argument is &
		{
			perror("wrong syntax near |\n");
		}
		else if(argv[index-1][strlen(argv[index-1])-1]=='&')
		{
			perror("wrong syntax near |\n");
		}
		dup2(pipe_in, 0); //change the input according to the old one 
		if (pipe_command[i+1] != NULL)
		{

			pipe(fd);
		}
		else
		{
			if(pipe_in!=0)
				dup2(pipe_in,0);


                        int f1=0,f2=1;
                        if (redirect)
                        {
                                if(in!=NULL)
                                {
                                        f1=open(in, O_RDONLY);
                                        dup2(f1,0);
                                        dup2(fd[1],1);
                                        close(f1);
                                        in=NULL;

                                }
                                if(out!=NULL)
                                {
                                        
                                        f2=open(out,O_WRONLY|O_CREAT,S_IRWXU);
                                        dup2(f2,1);
                                        close(f2);
                                        out=NULL;
                                }

                        } 
                        execvp(argv[0],argv);
		}
		if ((pid = fork()) == -1)
		{
			exit(EXIT_FAILURE);
		}
		else if(pid==0)
		{
			if(!redirect)
			{
				if(fd[0]!=0)
				{
					dup2(pipe_in,0);
					close(pipe_in);
				}

				if(fd[1]!=1)
				{
					dup2(fd[1],1);
					close(fd[1]);
				}
			}

			int f1=0,f2=1;

			if (redirect)
			{
				if(in!=NULL)
				{
					f1=open(in, O_RDONLY);
					dup2(f1,0);
                                        dup2(fd[1],1);
					close(f1);
				}
				if(out!=NULL)
				{

					f2=open(out,O_WRONLY|O_CREAT,S_IRWXU);
					dup2(f2,1);
				}

			} 

			execvp(argv[0], argv);



			exit(EXIT_FAILURE);
		}
		else
		{
			close(fd[1]);
			wait(NULL);
			pipe_in = fd[0]; //save the input for the next command
		}

	}

}
