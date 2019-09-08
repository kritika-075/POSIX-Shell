#include<iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define READ_BUFFER 1024
#define MAXSIZE 1024

char *arr[] = {"help","cd","exit"};

int change_dir(char **args);
int help_1(char **args);
int exit_1(char **args);

int change_dir(char **args)
{
		if(args[1]==NULL)
		fprintf(stderr,"expected argument to \"cd\"\n");
		else
		{
			if(chdir(args[1])!=0)
			printf("error\n");
			
		}
return 1;
}
int help_1(char **args)
	{
		printf("Hello\n,kritika singh");
		printf("\nType command and press enter key");
		printf("\nFor information refer to the man command");
	
return 1;
}
int exit_1(char **args)
{
return 0;
}
int execute(char **args)
{
  int i;
  if (args[0] == NULL) {
    return 1;
  }
      if (strcmp(args[0], "cd") == 0) 
      return change_dir(args);
			else if(strcmp(args[0], "help") == 0)
			return help_1(args);
			else if(strcmp(args[0], "exit") == 0)
			return exit_1(args);

else
{
	 pid_t pid, wpid;
		int stat;
		pid = fork();
		if (pid < 0)
	{
			printf("error");
			exit(EXIT_FAILURE);
	}
		else if (pid == 0) 
		{
				if (execvp(args[0], args) == -1)
				printf("error");
				exit(EXIT_FAILURE);
		} 
		else 
			wait(&stat);
}
return 1;
}

void prompt()
{
static int first=1;
	while(first)
	{
		const char* CLEAR_SCREEN="\e[1;1H\e[2J";
		write(STDOUT_FILENO,CLEAR_SCREEN,12);
		first=0;
	}
	printf("->");
}

char** separate_line(char *l)
{
	int buff=MAXSIZE;
	int pos=-1;
	char **split=(char **)malloc(buff * sizeof(char));
	char *tok;
	if(!split)
	{
		fprintf(stderr,"No Memory allocated");
		exit(EXIT_FAILURE);
	}
	tok=strtok(l," \t\r\n\a");
	while(tok!=NULL)
	{
		split[++pos]=tok;
		if(pos>=buff)
		{
			buff=buff+MAXSIZE;
			split=(char**)realloc(split,buff *sizeof(char));
			if(!split)
			{
			fprintf(stderr,"No memory allocated");
			exit(EXIT_FAILURE);
			}
		}
		tok=strtok(NULL," \t\r\n\a");
	}
	split[++pos]=NULL;
	return split;
}

char *get_line()
{
	int buff=MAXSIZE;int c;
	char * input=(char*)malloc(buff *sizeof(char));
	int p=-1;
	if(input==NULL)
	{
		fprintf(stderr, "ks: No Memory Allocated\n");
    exit(EXIT_FAILURE);
	}
	else
	{		
		while(true)
		{
			c=getchar();
			if(p>=(buff-1))
		  {
				buff=buff+MAXSIZE;
				input=(char *)realloc(input,buff);
				if(!input)
				fprintf(stderr, "No Memory Allocated\n");
				exit(EXIT_FAILURE);
		  }
			if(c== EOF ||c=='\n')
			{
				input[++p]='\0';
				return input;
			}
			else
			input[++p]=c;
	}	
}
}
int main(int argc,char **argv)
{
		int stat;
		char **arr;
		char *l;
		do
		{
			prompt();
			l=get_line();
			arr=separate_line(l);
			stat=execute(arr);
		}
		while(stat);
		return 1;
}







