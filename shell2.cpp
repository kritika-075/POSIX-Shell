#include<iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<fcntl.h>
#define READ_BUFFER 1024
#define MAXSIZE 1024

char *arr[] = {"help","cd","exit"};

int change_dir(char **args);
int help_1(char **args);
int exit_1(char **args);
int pipe_count=0;

void initialize()
{
	FILE *fd;
	fd=fopen("bash1.txt","w+");
	char *str1;char *str2;char *str3;char *str4;
	str1=getenv("PATH");
	str2=getenv("USER");
	str3=getenv("HOME");
	str4=getenv("USERNAME");
	fputs(str1,fd);
	fputs("\n",fd);
	fputs(str2,fd);
	fputs("\n",fd);
	fputs(str3,fd);
	fputs("\n",fd);
	fputs(str4,fd);
	fputs("\n",fd);
	fclose(fd);
}

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
int exe_pipe(char **arr)
{   
	int p=0;
	int fd[2];
	int pid;
	int c=0;
	while(*arr !=NULL)
	{
		pipe(fd);
		pid = fork();//creating a child process
		if(pid==-1)
		{
			printf("Error forking!\n");
			return 1;
		}
		else if(pid ==0)//child has been created
  		{
    	dup2(p,0);
    	if(*(arr+1) !=NULL)
    	dup2(fd[1],1);
    	close(fd[0]);
    	close(fd[1]);
    	char **inp;
    inp=separate_line(arr[c]);
    /// to check for exception commands like redirection chdir etc
    if(execvp(inp[0],inp)==-1)
    	printf("Command not executed!");
    return 1;
   }
   else
   {
   	wait(NULL);
   	close(fd[1]);
   	p=fd[0];
	arr++;
   	}	
	}
pipe_count=0;
return 1;
}

char** piped(char *l)
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
	tok=strtok(l,"|");
	while(tok!=NULL)
	{
		split[++pos]=tok;
		if(pos==(buff-1))
		{
			buff=buff+MAXSIZE;
			split=(char**)realloc(split,buff *sizeof(char));
			if(!split)
			{
			fprintf(stderr,"No memory allocated");
			exit(EXIT_FAILURE);
			}
		}
		tok=strtok(NULL,"|");
	}
	split[++pos]=NULL;
	return split;
}

char *get_line()
{
	int buff=MAXSIZE;int c;int redirection=0;
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
			if(c=='|')
			pipe_count=1;
			if(c=='>')
			redirection=1;
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
//HISTORY
void add_history(char *l)
{
	FILE *fp;
	fp=fopen("history.txt","a");
	fputs(l,fp);
	fclose(fp);
}
//MAIN FUNCTION HERE
int main(int argc,char **argv)
{
		int stat;
		initialize();
		do
		{
			char **arr;
		   char **split;
		   char *l;
			prompt();
			l=get_line();
			add_history(l);
			if(pipe_count!=0)
			{
				split=piped(l);
				stat=exe_pipe(split);
			}
		else
		{
		arr=separate_line(l);
		stat=execute(arr);
		}
		}
		while(stat);
		return 1;
}







