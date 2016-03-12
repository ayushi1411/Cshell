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
#define MAX_PAR 10  //max number of parameters 
#define MAX_CMD_LENGTH 100 //max length of the command
char * tempcommand;
int redirect=0;
void handler()//handler function
{
	pid_t pid;
	int status;
	if(pid=waitpid(-1,&status,WNOHANG)>0)
		if(WIFEXITED(status))
			printf("program %d terminated successfully\n",pid);
}

int main()
{
	char * pwd="~";//relative current directory
	while(1)
	{
		char * username=getenv("USER");//user of the system
		char * home=getenv("PWD");//calling directory

		printf("%s@UBUNTU %s >",username,pwd); //prints the prompt
		char command[MAX_CMD_LENGTH];//stores the command
		scanf("%[^\n]s",command);//reads the entire command
		tempcommand=strdup(command);

		char * indv_command[MAX_PAR];
		int iccount=0;
		char *token; 
		token=strtok(command,";");
		redirect=0;
		while(token!=NULL)
		{
			indv_command[iccount]=strdup(token);
			iccount++;
			token=strtok(NULL,";");
		}

		int i=0;
		for(i=0;i<iccount;i++)
		{
			char * pipe_command[MAX_PAR];
			int pccount=0;
			char *token1;
			token1=strtok(indv_command[i],"|");
			while(token1!=NULL)
			{
				pipe_command[pccount]=strdup(token1);
				pccount++;
				token1=strtok(NULL,"|");
			}
			pipe_command[pccount]=NULL;
			redirect=check_redirect(pipe_command[0]);

			int index=0;
			int bg_flag=0;
			while(argv[index]!=NULL)
			{
				index++;
			}
			if(strcmp(argv[index-1],"&")==0)//if the last argument is &
			{
				argv[index-1]=NULL;//making the arg null
				bg_flag=1;//background process wanted
			}
			else if(argv[index-1][strlen(argv[index-1])-1]=='&')//if the last character of the arg is &
			{
				argv[index-1]=strndup(argv[index-1],strlen(argv[index-1])-1);//delete the &
				bg_flag=1;//background process wanted
			}
			if(bg_flag==0)//if background process is not wanted
				argv[index]=NULL;

			if (strcmp(argv[0],"exit")==0)//if the command is exit
			{
				printf("exiting the program\n");
				exit(0);
			}
			else if(strcmp(argv[0],"cd")==0)//if the command is cd
			{
				char temppwd[MAX_CMD_LENGTH];
				getcwd(temppwd,MAX_CMD_LENGTH);//gets the absolute current directory
				if (argv[1]==NULL)//if change directory to home
				{
					chdir(home);//changes to relative home directory
					pwd="~";
				}
				else if(strcmp(argv[1],"~")==0)
				{
					chdir(home);
					pwd="~";
				}
				else if (strcmp(home,temppwd)==0 && strcmp(argv[1],"..")==0)//change to parent directory of home then change to root
				{
					chdir("/");
					pwd="/";
				}



				else if(chdir(argv[1])==0)//otherwise
				{
					getcwd(temppwd,MAX_CMD_LENGTH);
					if(strcmp(temppwd,home)!=0)//compares if the new directory is not home directory
					{
						pwd=strdup(temppwd);
						pwd=pwd+strlen(home);//change the pwd 
					}
					else
						pwd="~";


				}
				else
					perror("MY SHELL");


			}
			else if(strcmp(argv[0],"echo")==0)//echo command
			{
				char str[MAX_CMD_LENGTH]="";
				int k=1;
				while(argv[k]!=NULL)//til the arguments of echo are not null
				{
					strcat(str,argv[k]);//storing it as a string 
					k++;
				}

				int flag=-1;//to check the termination of ""

				char input[1000]="";//final output to be printed
				k=0;
				int j;
				for(j=0;j<strlen(str);j++)
				{
					if (str[j]=='\"')
						flag*=-1;
					else
					{
						input[k]=str[j];//stores the string except ""
						k++;
					}	

				}
				while(flag==1)
				{
					printf(" > ");//if "" not terminated, generates the prompt
					char c;
					getchar();
					while ((c=getchar())!='\n')
					{
						if(c=='\"')
							flag*=-1;
						else
						{
							input[k]=c;
							k++;
						}
					}

				}
				printf("%s\n",input);//outputs the entire input

			}
			else if(strcmp(argv[0],"pwd")==0)//if command is pwd
			{
				printf("%s\n",pwd);//display the pwd
			}

			else//otherwise
			{
				int pid=fork();//create child process
				if(pid<0)
					printf("child process could not be created\n");
				else if(pid==0)
				{
					if(pccount!=0)
					{
						piping(pipe_command,pccount);
					}
					else
					{
						int f1=0,f2=1;

						if (redirect)
						{
							if(in!=NULL)
							{
								f1=open(in, O_RDONLY);
								dup2(f1,0);
							}
							if(out!=NULL)
							{
								f2=open(out,O_WRONLY|O_CREAT,S_IRWXU);
								dup2(f2,1);
							}

						}	
						if(f1!=0)
							close(f1);
						if(f2!=1)
							close(f2);
						signal(SIGCHLD,handler);
						execvp(argv[0],argv);//execute command
					}
				}
				else
				{
					printf("bg_flag: %d\n",bg_flag);
					if(bg_flag==0)
					{
						waitpid(pid,0,0);
					}
				}
			}
			i=0;
			int k;
			for( k=0;k<MAX_PAR;k++)
				argv[k]=NULL;



		}
		getchar();




	}
	return 0;
}
