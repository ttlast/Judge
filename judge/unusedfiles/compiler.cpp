/*
 * a compiler for online judge
 * Author: TT_last
 * 
 * c c++ java
 */
#ifndef __COMPLIER__
#define __COMPLIER__

#include "judge.h"
using namespace std;

extern int errno;

int Compiler(int lang)
{
	signal(SIGALRM, timeout);

	const char *CP_C[] = {"gcc","Main.c","-o","Main", 
		"-static", "-w", "-lm", "-std=c99", "-O2", "-DOJ",NULL};
	const char *CP_X[] = {"g++","Main.cpp","-o","Main",
		 "-static", "-w", "-O2", "-DOJ",NULL};
	const char *CP_J[] = {"javac", "-J-Xms32m", "-J-Xmx256m", "Main.java",
			NULL };
	
	int status = 0;
	pid_t compiler = fork();
	if(compiler < 0)
	{
		LOG_WARNING("error fork compiler");
		exit(judge_conf::EXIT_COMPILE);
	}else if(compiler == 0)
	{
		malarm(ITIMER_REAL, judge_conf::compile_time_limit);
		switch(lang)
		{
			case judge_conf::LANG_C:
				execvp(CP_C[0],(char * const *) CP_C);
				break;
			case judge_conf::LANG_CPP:
				execvp(CP_X[0],(char * const *) CP_X);
				break;
			case judge_conf::LANG_JAVA:
				execvp(CP_J[0],(char * const *) CP_J);
				break;
		}
		exit(judge_conf::EXIT_COMPILE);
	}else{
		pid_t w = waitpid(compiler,&status,WUNTRACED);
		if(w == -1)
			exit(judge_conf::EXIT_COMPILE);
		if(WIFEXITED(status))
		{
			if(EXIT_SUCCESS == WEXITSTATUS(status))
			{
				printf("exit success\n");
			}else{
				printf("exit error\n");
			}
		}
	}
}

#endif

//Compiler 需要： 语言  源文件名	输出文件名
int main(int argc,char *argv[],char *envp[])
{
	
	return 0;
}
