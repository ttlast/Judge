#ifndef __JUDGE_H__
#define __JUDGE_H__

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <string>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <pwd.h>
#include <sys/types.h>
#include "logger.h"

namespace judge_conf
{
	//配置文件名
	const std::string config_file = "config.ini";

	//日志文件路径
	std::string log_file = "log.txt";

	//judge 自己时限(ms) 40s
	int judge_time_limit = 40000;

	//编译时限(ms) 8s
	int compile_time_limit = 8000;

	//spj时限(ms) 10s
	int spj_time_limit = 10000;

	//程序运行栈空间大小(KB) 8M
	int stack_size_limit = 8192;

	//时间误差，防止限制太死.
	int time_limit_addtion = 400;
	//java 时间、内存翻倍
	int java_time_factor = 2;
	int java_memory_factor = 2;

	//OJ语言
	const std::string languages[] = {"unknown","c","c++","java"};
	const int LANG_UNKNOWN		= 0;
	const int LANG_C			= 1;
	const int LANG_CPP			= 2;
	const int LANG_JAVA			= 3;

	//OJ测试结果
	const int OJ_WAIT	= 0; 	//Queue
	const int OJ_AC		= 1;	//AC
	const int OJ_PE		= 2;	//PE
	const int OJ_TLE	= 3;	//TLE
	const int OJ_MLE	= 4;	//MLE
	const int OJ_WA		= 5;	//WA
	const int OJ_OLE	= 6;	//OLE
	const int OJ_CE		= 7;	//CE
	const int OJ_RE_SEGV	= 8;	//SEG Violation
	const int OJ_RE_FPU		= 9;	//float.../0
	const int OJ_RE_ABRT	= 10;	//Abort
	const int OJ_RE_UNKNOW	= 11;	//Unknow
	const int OJ_RF		= 12;	//Restricted Function
	const int OJ_SE		= 13;	//System Error
	const int OJ_RE_JAVA	= 15;	//Java Run Time Exception

	//内存衡量单位
	const int KILO		= 1024;		//1K
	const int MEGA		= KILO*KILO;	//1M	
	const int GIGA		= KILO*MEGA;	//1G

	//judge退出原因
    const int EXIT_OK               = 0;
    const int EXIT_UNPRIVILEGED     = 1;
    const int EXIT_BAD_PARAM        = 3;
    const int EXIT_VERY_FIRST       = 4;
    const int EXIT_COMPILE          = 6;
    const int EXIT_PRE_JUDGE        = 9;
    const int EXIT_PRE_JUDGE_PTRACE = 10;
    const int EXIT_PRE_JUDGE_EXECLP = 11;
    const int EXIT_SET_LIMIT        = 15;
    const int EXIT_SET_SECURITY     = 17;
    const int EXIT_JUDGE            = 21;
    const int EXIT_COMPARE          = 27;
    const int EXIT_COMPARE_SPJ      = 30;
    const int EXIT_COMPARE_SPJ_FORK = 31;
    const int EXIT_TIMEOUT          = 36;
    const int EXIT_UNKNOWN          = 127;


};

namespace problem
{
	int id = 0;
	int lang = 0;	//提交语言
	int time_limit = 1000;
	int memory_limit = 65536;
	int output_limit = 8192;
	int result = 0;  //测试结果
	int status;

	long memory_usage = 0;
	int time_usage = 0;
	bool spj = false;

	std::string uid;
	std::string temp_dir;	//临时目录
	std::string data_dir;	//数据目录

	//出错信息 debug 用
	std::string stdout_file_compiler;	
	std::string stderr_file_complier;
	std::string stdout_file_executive;
	std::string stderr_file_executive;


	std::string source_file;	//源文件
	std::string exec_file;		//可执行文件
	std::string spj_exe_file;	//spj可执行文件

	std::string data_file;	//数据文件
	std::string input_file;	//输入文件
	std::string output_file_std;	//输出文件
	std::string stdout_file_spj;	//spj输出文件


#ifdef JUDGE_DEBUG
	void Problem_debug()
	{
		LOG_DEBUG("----Problem\tinformation----");
		LOG_DEBUG("id %d lang %s",id,judge_conf::languages[lang].c_str());
		LOG_DEBUG("time_limit %d	memory_limit %d",time_limit,memory_limit);
		LOG_DEBUG("output_limit %d",output_limit);

		LOG_DEBUG("temp_dir %s",temp_dir.c_str());
		LOG_DEBUG("data_dir %s",data_dir.c_str());
		LOG_DEBUG("source_file %s",source_file.c_str());
		LOG_DEBUG("data_file %s\n",data_file.c_str());
	}
#endif
};

void timeout(int signo)
{
	printf("time is used\n");
	printf("exit\n");
	if(signo == SIGALRM)
		exit(judge_conf::EXIT_TIMEOUT);
}

//真实时间	进程执行用户时间	进程执行总时间
//which 表示某个时间： ITIMER_REAL	ITIMER_VIRTUAL	ITIMER_PROF
// example:malarm(ITIMER_REAL,judge_conf::judge_time_limit);
int malarm(int which,int milliseconds)
{
	struct itimerval it;
	it.it_value.tv_sec = milliseconds/1000;
	it.it_value.tv_usec = (milliseconds%1000)*1000; //微秒
	it.it_interval.tv_sec = 0;
	it.it_interval.tv_usec = 0;
	return setitimer(which, &it,NULL);
}

#endif


int main()
{
	signal(SIGALRM,timeout);
	//malarm(ITIMER_REAL,1000);
	sleep(10);
	return 0;
}
