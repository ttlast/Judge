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

//#define JUDGE_DEBUG
namespace judge_conf
{
	//ÅäÖÃÎÄ¼þÃû
	const std::string config_file = "config.ini";

	//ÈÕÖ¾ÎÄ¼þÂ·¾¶
	std::string log_file = "log.txt";

	//judge ×Ô¼ºÊ±ÏÞ(ms) 40s
	int judge_time_limit = 40000;

	//¶ÑÕ»´óÐ¡ÉèÖÃ£º
	int stack_size_limit = 8192;

	//±àÒëÊ±ÏÞ(ms) 8s
	int compile_time_limit = 8000;

	//spjÊ±ÏÞ(ms) 10s
	int spj_time_limit = 10000;

	//Ê±¼äÎó²î£¬·ÀÖ¹ÏÞÖÆÌ«ËÀ.
	int time_limit_addtion = 400;
	//java Ê±¼ä¡¢ÄÚ´æ·­±¶
	int java_time_factor = 2;
	int java_memory_factor = 2;

	//OJÓïÑÔ
	const std::string languages[] = {"unknown","c","c++","java"};
	const int LANG_UNKNOWN		= 0;
	const int LANG_C			= 1;
	const int LANG_CPP			= 2;
	const int LANG_JAVA			= 3;

	//OJ²âÊÔ½á¹û
	const int OJ_WAIT	= 0; 	//Queue
	const int OJ_RUN	= 1;	//RUN
	const int OJ_AC		= 2;	//AC
	const int OJ_PE		= 3;	//PE
	const int OJ_TLE	= 4;	//TLE
	const int OJ_MLE	= 5;	//MLE
	const int OJ_WA		= 6;	//WA
	const int OJ_OLE	= 7;	//OLE
	const int OJ_CE		= 8;	//CE
	const int OJ_RE_SEGV	= 9;	//SEG Violation
	const int OJ_RE_FPU		= 10;	//float.../0
	const int OJ_RE_ABRT	= 11;	//Abort
	const int OJ_RE_UNKNOW	= 12;	//Unknow
	const int OJ_RF		= 13;	//Restricted Function
	const int OJ_SE		= 14;	//System Error
	const int OJ_RE_JAVA	= 15;	//Java Run Time Exception

	//ÄÚ´æºâÁ¿µ¥Î»
	const int KILO		= 1024;		//1K
	const int MEGA		= KILO*KILO;	//1M	
	const int GIGA		= KILO*MEGA;	//1G

	//judgeÍË³öÔ­Òò
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
	int lang = 0;	//Ìá½»ÓïÑÔ
	int time_limit = 1000;
	int memory_limit = 65536;
	int output_limit = 8192;
	int result = judge_conf::OJ_SE;  //²âÊÔ½á¹û
	int status;

	long memory_usage = 0;
	long time_usage = 0;
	bool tc = false;  
	bool spj = false;

	std::string uid;
	std::string temp_dir	= "temp";	//ÁÙÊ±Ä¿Â¼
	std::string data_dir	= "data";	//Êý¾ÝÄ¿Â¼


	std::string source_file;	//Ô´ÎÄ¼þ
	std::string tc_file;		//tcÄ£Ê½ÎÄ¼þ
	std::string spj_exe_file;	//spj¿ÉÖ´ÐÐÎÄ¼þ

	std::string input_file;	//ÊäÈëÎÄ¼þ
	std::string output_file;
	std::string output_file_std;	//Êä³öÎÄ¼þ
	std::string stdout_file_spj;	//spjÊä³öÎÄ¼þ


#ifdef JUDGE_DEBUG
	void Problem_debug()
	{
		LOG_DEBUG("----Problem\tinformation----");
		LOG_DEBUG("Problem spj : %s Problem tc %s",spj?"True":"False",tc?"True":"False");
		LOG_DEBUG("id %d lang %s",id,judge_conf::languages[lang].c_str());
		LOG_DEBUG("time_limit %d	memory_limit %d",time_limit,memory_limit);
		LOG_DEBUG("output_limit %d",output_limit);

		LOG_DEBUG("temp_dir %s",temp_dir.c_str());
		LOG_DEBUG("data_dir %s",data_dir.c_str());

		LOG_DEBUG("input_file %s",input_file.c_str());
		LOG_DEBUG("output_file %s",output_file.c_str());
		LOG_DEBUG("output_file_std %s",output_file_std.c_str());
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

//ÕæÊµÊ±¼ä	½ø³ÌÖ´ÐÐÓÃ»§Ì¬ ÄÚºËÌ¬Ê±¼ä	½ø³ÌÖ´ÐÐ×ÜÊ±¼ä £¬ÉèÖÃºÃÏûÏ¢¡£
//ÕâÀïÓÃÕæÊµÊ±¼ä£¬·ÀÖ¹ÓÃ»§ÓÃcons sleepµÈ ¿¨ËÀjudge
//which ±íÊ¾Ä³¸öÊ±¼ä£º ITIMER_REAL	ITIMER_VIRTUAL	ITIMER_PROF
// example:malarm(ITIMER_REAL,judge_conf::judge_time_limit);
int malarm(int which,int milliseconds)
{
	struct itimerval it;
	it.it_value.tv_sec = milliseconds/1000;
	it.it_value.tv_usec = (milliseconds%1000)*1000; //Î¢Ãë
	it.it_interval.tv_sec = 0;
	it.it_interval.tv_usec = 0;
	return setitimer(which, &it,NULL);
}

#endif


