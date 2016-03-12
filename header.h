#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#define MAX_PAR 10  //max number of parameters 
#define MAX_CMD_LENGTH 100 //max length of the command
struct job{

	char * process;
	int pid;
	int id;
}jobs[1000];
//int proc_index=0;
//extern int redirect;
void piping(char * pipe_command[],int count);
int check_redirect(char * pipe_command);
char * in;
char * out;
char * argv[MAX_CMD_LENGTH];
#endif 

