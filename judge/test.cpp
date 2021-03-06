/*
 * A simple judge
 * Author: TT_last
 * 
 * use:
 *    ./judge -l 语言   -D 数据目录
 *            -d 临时目录 -t 时间限制 -m 内存限制 -o 输出限制  [-S dd] [-T]
 *	-l 语言        C=1, C++=2, JAVA=3
 *	-D 数据目录	   包括题号的输入输出文件的目录
 *	-d 临时目录    judge可以用来存放编译后的文件及其他临时文件的
 *	-t 时间限制    允许程序运行的最长时间, 以毫秒为单位, 默认为1000, 1s
 *	-m 内存限制    允许程序使用的最大内存量, 以KB为单位, 默认为65536, 64MB
 *	-o 输出限制    允许程序输出的最大数据量, 以KB为单位, 默认为8192, 8MB
 *	-S dd          可选, 如指定此参数, 则表示这是一个Special Judge的题目   
 *	-T			   可选，如指定此参数，则表示这个topcoder模式题目，只要
 *				   写相关头文件 和 相关类+接口就可以了。 ^-^
 *
 *	其中-S dd和 -T可以混合用.
 *
 * 用-S dd，data需要有spj.exe spj.cpp
 * 用-T 则相应的需要tc.c tc.cpp tc.java之类的配套
 * for example:
 * ./judge -l 2 -D data -d temp -t 1000 -m 65536 -o 8192
 *
 * */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/time.h>
#include "judge.h"
#include "okcall.h"

#define Max(x,y) (x) > (y) ? (x) : (y)
using namespace std;
//#define JUDGE_DEBUG
extern int errno;
const int MAXN = 8192;

//normal compare file
void compare_until_nonspace(int &c_std,int &c_usr,
		FILE *&fd_std,FILE *&fd_usr,int &ret)
{
	while((isspace(c_std)) || (isspace(c_usr))){
		if(c_std != c_usr){
			if(c_std == '\r' && c_usr == '\n'){
				c_std = fgetc(fd_std);
				if(c_std != c_usr)
					ret = judge_conf::OJ_PE;
			}else ret = judge_conf::OJ_PE;
		}
		if(isspace(c_std))
			c_std = fgetc(fd_std);
		if(isspace(c_usr))
			c_usr = fgetc(fd_usr);
	}
}

//tc模式
void addfile(string &main_file,string &tc_file){
	LOG_DEBUG("TC mode");
	char cc[MAXN+5];
	FILE *sc_fd = fopen(main_file.c_str(),"a");
	FILE *tc_fd = fopen(tc_file.c_str(),"r");
	if((!sc_fd) || (!tc_fd)){
		exit(judge_conf::EXIT_PRE_JUDGE);
	}else{
		while(fgets(cc,MAXN,tc_fd)){
			fputs(cc,sc_fd);
		}
	}
	if(sc_fd)
		fclose(sc_fd);
	if(tc_fd)
		fclose(tc_fd);
}

int tc_mode(){
	switch(problem::lang){
		case judge_conf::LANG_C:
			problem::source_file = problem::temp_dir + "/" + "Main.c";
			break;
		case judge_conf::LANG_CPP:
			problem::source_file = problem::temp_dir + "/" + "Main.cpp";
			break;
		case judge_conf::LANG_JAVA:
			problem::source_file = problem::temp_dir + "/" + "Main.java";
			break;
		default:
			break;
	}
	addfile(problem::source_file,problem::tc_file);
	return -1;
}

//特判
int spj_compare_output(
		string input_file,	//标准输入文件
		string output_file,	//用户程序的输出文件
		string spj_exec,	//spj路径
		string file_spj		//spj的输出文件
		)
{
#ifdef JUDGE_DEBUG__
	cout<<"inputfile: "<<input_file<<endl;
	cout<<"outputfile: "<<output_file<<endl;
	cout<<"spj_exec: "<<spj_exec<<endl;
	cout<<"file_spj: "<<file_spj<<endl;
#endif
	int status = 0;
	pid_t pid_spj = fork();
	if(pid_spj < 0){
		LOG_WARNING("error for spj failed");
		exit(judge_conf::EXIT_COMPARE_SPJ);
	}else if(pid_spj == 0){

		freopen(file_spj.c_str(),"w",stdout);

		if(EXIT_SUCCESS == malarm(ITIMER_REAL,judge_conf::spj_time_limit))
		{
			log_close();
			//argv[1] 标准输入 ， argv[2] 用户程序输出
			if(execlp(spj_exec.c_str(),"spj.exe",input_file.c_str(),
						output_file.c_str(),NULL) < 0)
			{
				printf("spj execlp error\n");
			}
		}
	}else{
		if(waitpid(pid_spj,&status,0) < 0){
			exit(judge_conf::EXIT_COMPARE_SPJ);
		}

		if(WIFEXITED(status)){
			if(WEXITSTATUS(status) == EXIT_SUCCESS){
				FILE *fd = fopen(file_spj.c_str(),"r");
				if(fd == NULL)
					exit(judge_conf::EXIT_COMPARE_SPJ);
				char buf[20];
				if(fscanf(fd,"%19s",buf) == EOF){
					return judge_conf::OJ_WA;
				}
				if(fd) fclose(fd);
				if(strcmp(buf,"AC") == 0){
					return judge_conf::OJ_AC;
				}else if(strcmp(buf,"PE") == 0){
					return judge_conf::OJ_PE;
				}else if(strcmp(buf,"WA") == 0){
					return judge_conf::OJ_WA;
				}else return judge_conf::EXIT_COMPARE_SPJ;
			}
		}
	}
	exit(judge_conf::EXIT_COMPARE_SPJ);
}

//普通比较
int tt_compare_output(string &file_std,string &file_usr)
{
	int ret = judge_conf::OJ_AC;
	int c_std,c_usr;
	FILE *fd_std = fopen(file_std.c_str(),"r");
	FILE *fd_usr = fopen(file_usr.c_str(),"r");
	//char *buffer;
		//buffer = getcwd(NULL,0);
		//if(buffer != NULL) LOG_DEBUG("this is %s",buffer);

	//LOG_DEBUG("The standard file is %s",file_std.c_str());
	if(fd_std == NULL){
		LOG_BUG("%s open standard file failed %s",strerror(errno),file_std.c_str());
	}
	//if(fd_usr == NULL){
		//LOG_BUG("open user file failed %s",file_usr.c_str());
	//}

	if((!fd_std) || (!fd_usr)){
		//LOG_DEBUG("compare This is the file: %s\n",problem::input_file.c_str());
		ret = judge_conf::OJ_RE_ABRT;
	}else{
		c_std = fgetc(fd_std);
		c_usr = fgetc(fd_usr);
		for(;;){
			compare_until_nonspace(c_std,c_usr,fd_std,fd_usr,ret);
			while((!isspace(c_std)) && (!isspace(c_usr))){
			//	LOG_DEBUG("std: %c usr: %c",c_std,c_usr);
				if(c_std == EOF && c_usr == EOF)
					goto end;
				if(c_std != c_usr){
					ret = judge_conf::OJ_WA;
					goto end;
				}
				c_std = fgetc(fd_std);
				c_usr = fgetc(fd_usr);
			}
		}
	}
end:
	if(fd_std)
		fclose(fd_std);
	if(fd_usr)
		fclose(fd_usr);
	return ret;
}

int compare_output(string &file_std,string &file_usr)
{
//	LOG_DEBUG("compare_output ");
	return tt_compare_output(file_std,file_usr);
}

void output_result(int result,int memory_usage = 0,int time_usage = 0)
{
	//#ifdef JUDGE_DEBUG
	LOG_DEBUG("result: %d, %dKB %dms",result,memory_usage,time_usage);
	//#endif
	printf("%d %d %d\n",result,memory_usage,time_usage);
}

void parse_arguments(int argc,char *argv[])	//根据命令设置好配置.
{
	int opt;
	extern char *optarg;
	while((opt = getopt(argc,argv,"l:D:d:t:m:o:S:T")) != -1)
	{
		//LOG_DEBUG(" -%c",opt);
		switch(opt){
			case 'l': sscanf(optarg,"%d",&problem::lang); break;
			case 'D': problem::data_dir = optarg; break;
			case 'd': problem::temp_dir = optarg; break;
			case 't': sscanf(optarg,"%d",&problem::time_limit); break;
			case 'm': sscanf(optarg,"%d",&problem::memory_limit); break;
			case 'o': sscanf(optarg,"%d",&problem::output_limit); break;
			case 'S': problem::spj = true; break;
			case 'T': problem::tc = true; break;
			default:
					  LOG_WARNING("unknown option -%c %s",opt,optarg);
					  exit(judge_conf::EXIT_BAD_PARAM);
		}
	}

	if(problem::lang == judge_conf::LANG_JAVA)
	{
		problem::time_limit *= judge_conf::java_time_factor;
		problem::memory_limit *= judge_conf::java_memory_factor;
	}
	if(problem::tc){
		if(problem::lang == judge_conf::LANG_JAVA){
			problem::tc_file = problem::data_dir + "/" + "tc.java";
		}else if(problem::lang == judge_conf::LANG_C){
			problem::tc_file = problem::data_dir + "/" + "tc.c";
		}else if(problem::lang == judge_conf::LANG_CPP){
			problem::tc_file = problem::data_dir + "/" + "tc.cpp";
		}
	}
	if(problem::spj == true)
	{
		problem::spj_exe_file = problem::data_dir + "/" + "spj.exe";
		problem::stdout_file_spj = problem::temp_dir + "/" + "stdout_spj.txt";
	}
	judge_conf::judge_time_limit += problem::time_limit;
#ifdef JUDGE_DEBUG
	problem::Problem_debug();
#endif
}


//redirect io
void io_redirect()
{
	freopen(problem::input_file.c_str(),"r",stdin);
	freopen(problem::output_file.c_str(),"w",stdout);
	if(stdin == NULL || stdout == NULL){
		LOG_BUG("error in freopen: stdin(%p) stdout(%p)",stdin,stdout);
		exit(judge_conf::EXIT_PRE_JUDGE);
	}
#ifdef JUDGE_DEBUG
	LOG_DEBUG("io redirece ok!");
#endif
}

void set_limit()
{
	rlimit lim;
	//时间限制
	lim.rlim_max = (problem::time_limit - problem::time_usage + 999)/1000 + 1;
	lim.rlim_cur = lim.rlim_max;
	if(setrlimit(RLIMIT_CPU,&lim) < 0){
		LOG_BUG("error setrlimit for rlimit_cpu");
		exit(judge_conf::EXIT_SET_LIMIT);
	}

	//内存大小限制 java 无法运行
	/*
	   if(problem::lang != judge_conf::LANG_JAVA)
	   {
	   lim.rlim_max = problem::memory_limit * judge_conf::KILO;
	   }else lim.rlim_max = (problem::memory_limit + 8192)*judge_conf::KILO;
	   lim.rlim_cur = lim.rlim_max;
	   if(setrlimit(RLIMIT_AS,&lim) < 0){
	   LOG_BUG("error setrlimit for rlimit_as");
	   exit(judge_conf::EXIT_SET_LIMIT);
	   }*/
	//设置堆栈的大小  漏掉主程序会SIGSEGV
	getrlimit(RLIMIT_STACK,&lim);
	int rlim = judge_conf::stack_size_limit*judge_conf::KILO;
//	LOG_DEBUG("set stack size : %d",rlim);
	if (lim.rlim_max <= rlim)
	{
		LOG_WARNING("can't set stack size to higher(%d <= %d)",
				lim.rlim_max, rlim);
	}
	else
	{
		lim.rlim_max = rlim;
		lim.rlim_cur = rlim;
		if (setrlimit(RLIMIT_STACK, &lim) < 0)
		{
			LOG_WARNING("setrlimit RLIMIT_STACK failed: %s", strerror(errno));
			exit(judge_conf::EXIT_SET_LIMIT);
		}
	}

	log_close();

	//输出文件限制
	lim.rlim_max = problem::output_limit * judge_conf::KILO;
	lim.rlim_cur = lim.rlim_max;
	//LOG_BUG("rlim_fsize %d",lim.rlim_max);
	if(setrlimit(RLIMIT_FSIZE,&lim) < 0){
		//	LOG_BUG("error setrlimit for rlimit_fsize");
		exit(judge_conf::EXIT_SET_LIMIT);
	}
	//log_close();
}

int Compiler()
{
#ifdef JUDGE_DEBUG
	const char *CP_C[] = {"gcc","Main.c","-o","Main", 
		"-std=c99", "-O2", NULL};
	const char *CP_X[] = {"g++","Main.cpp","-o","Main", 
		"-O2",NULL};
	const char *CP_J[] = {"javac", "Main.java",
		NULL };

#else		//用户程序必须是静态的，系统调用的限制才能有效工作。
	const char * CP_C[] = { "gcc", "Main.c", "-o", "Main", "-O2","-Wall", "-lm",
		"--static", "-std=c99", "-DONLINE_JUDGE", NULL };
	const char * CP_X[] = { "g++", "Main.cpp", "-o", "Main", "-O2", "-Wall",
		"-lm", "--static", "-DONLINE_JUDGE", NULL };
	const char *CP_J[] = {"javac", "-J-Xms32m", "-J-Xmx256m", "Main.java",
		NULL };
#endif
	int status = 0;
	pid_t compiler = fork();
	if(compiler < 0)
	{
		LOG_WARNING("error fork compiler");
		exit(judge_conf::EXIT_COMPILE);
	}else if(compiler == 0)
	{
		chdir(problem::temp_dir.c_str());

		freopen("ce.txt","w",stderr); //编译出错信息
		malarm(ITIMER_REAL,judge_conf::compile_time_limit);
		switch(problem::lang)
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
		//execvp  error
		LOG_WARNING("compile evecvp error");
		exit(judge_conf::EXIT_COMPILE); 
	}else{
		waitpid(compiler,&status,0);
		return status;
	}
}

const int bufsize = 1024;
int getmemory(pid_t userexe)
{
	int ret = 0;
	FILE *pd;
	char fn[bufsize],buf[bufsize];
	sprintf(fn,"/proc/%d/status",userexe);
	pd = fopen(fn,"r");
	while(pd && fgets(buf,bufsize-1,pd)){  //这里漏掉了pd & 导致主进程SIGSEGV
		if(strncmp(buf,"VmPeak:",7) == 0){
			sscanf(buf+8,"%d",&ret);
		}
	}
	if(pd) fclose(pd); //9.19 发现漏了，会too many open file
	return ret;
}

bool isInFile(char *filename)
{
	int len = strlen(filename);
	if(len < 3 || strcmp(filename+len-3,".in") != 0)
		return false;
	return true;
}

void sigseg(int){
	output_result(judge_conf::OJ_SE,0,0);
	exit(1);
}

int main(int argc,char *argv[])
{
	log_open(judge_conf::log_file.c_str());

#ifdef JUDGE_DEBUG
	LOG_DEBUG("start judge");
#endif
	if(geteuid() != 0)
	{
#ifdef JUDGE_DEBUG
		LOG_DEBUG("euid %d != 0,please run as root ,or chmod 4755 judge",geteuid());
#endif
		exit(judge_conf::EXIT_UNPRIVILEGED);
	}
	parse_arguments(argc,argv);
	//设置judge运行时限
	if(EXIT_SUCCESS != malarm(ITIMER_REAL,judge_conf::judge_time_limit))
	{
		LOG_WARNING("set judge alarm failed,%d : %s",errno,strerror(errno));
		exit(judge_conf::EXIT_VERY_FIRST);
	}
	signal(SIGALRM,timeout);

	//tc 模式
	if(problem::tc){
		tc_mode();
	}

	//编译
	int compile_ok = Compiler();
	if(compile_ok != 0) //测试结果OJ_CE
	{
		output_result(judge_conf::OJ_CE);
		exit(judge_conf::EXIT_OK);
	}

	//运行 judge
	DIR * dp;
	struct dirent *dirp;
	dp = opendir(problem::data_dir.c_str());
	if(dp == NULL)
	{
		LOG_WARNING("error opening dir %s",problem::data_dir.c_str());
		problem::result = judge_conf::OJ_SE;
		output_result(problem::result,problem::memory_usage,problem::time_usage);
		return judge_conf::EXIT_PRE_JUDGE;
	}
	char nametmp[1024];
	while((dirp = readdir(dp)) != NULL)
	{
		struct rusage rused;

		if(strcmp(dirp->d_name,".") == 0 || strcmp(dirp->d_name,"..") == 0)
			continue;
		if(isInFile(dirp->d_name))
		{
			strcpy(nametmp,dirp->d_name);
			int len = strlen(nametmp);
			nametmp[len-3] = '\0';
			problem::input_file = problem::data_dir + "/" + nametmp + ".in";
			problem::output_file_std = problem::data_dir + "/" + nametmp + ".out";
			problem::output_file = problem::temp_dir + "/" + nametmp + ".out";

#ifdef JUDGE_DEBUG
			problem::Problem_debug();
#endif

			pid_t userexe = fork();
			if(userexe < 0){
				LOG_WARNING("fork for userexe failed");
				exit(judge_conf::EXIT_PRE_JUDGE);
			}else if(userexe == 0){
				//重新定向io
				io_redirect();


				//设置运行根目录、运行用户
				//获得运行用户的信息
				struct passwd *judge = getpwnam("ttlast");
				if(judge == NULL){
					LOG_BUG("no user named 'judge'");
					exit(judge_conf::EXIT_SET_SECURITY);
				}

				//切换目录
				if(EXIT_SUCCESS != chdir(problem::temp_dir.c_str())){
					LOG_BUG("chdir failed");
					exit(judge_conf::EXIT_SET_SECURITY);
				}
				char cwd[1024], *tmp = getcwd(cwd,1024);
				if(tmp == NULL){ //获取当前目录失败
					LOG_BUG("getcwd failed");
					exit(judge_conf::EXIT_SET_SECURITY);
				}

				//#ifdef JUDEG_DEBUG
				//这里现在在fedora下有bug
				//设置根目录
				if(problem::lang != judge_conf::LANG_JAVA){
					if(EXIT_SUCCESS != chroot(cwd)){
						LOG_BUG("chroot failed");
						exit(judge_conf::EXIT_SET_SECURITY);
					}
				}
				//#endif

				//设置有效用户
				if(EXIT_SUCCESS != setuid(judge->pw_uid)){
					LOG_BUG("setuid failed");
					exit(judge_conf::EXIT_SET_SECURITY);
				}


				int user_time_limit = problem::time_limit - problem::time_usage
					+ judge_conf::time_limit_addtion;

				//设置用户程序的运行实际时间，防止意外情况卡住
				if(EXIT_SUCCESS != malarm(ITIMER_REAL,user_time_limit))
				{
					LOG_WARNING("malarm failed");
					exit(judge_conf::EXIT_PRE_JUDGE);
				}

				//用setrlimit 设置用户程序的 内存 时间 输出文件大小的限制
				//log close for fsize
				set_limit();


				//ptrace 监控下面程序
				if(ptrace(PTRACE_TRACEME,0,NULL,NULL) < 0){
					exit(judge_conf::EXIT_PRE_JUDGE_PTRACE);
				}


				//运行程序
				if(problem::lang != judge_conf::LANG_JAVA){
					if(execlp("./Main","Main",NULL) < 0)
					{
						printf("error exit_pre_jduge_execlp");
					}
				}else{
					execlp("java","java","Main",NULL);
					//这样运行有安全漏洞
				}
				exit(judge_conf::EXIT_PRE_JUDGE_EXECLP);
			}else{
				//父进程监控子进程的状态和系统调用
				signal(SIGSEGV,sigseg);

				int status = 0;
				int syscall_id = 0;
				struct user_regs_struct regs;

				init_ok_table(problem::lang);
				for(;;){
					if(wait4(userexe,&status,0,&rused) < 0)
					{
						LOG_BUG("wait4 failed, %d:%s",errno,strerror(errno));
						exit(judge_conf::EXIT_JUDGE);
					}
					//如果是退出信号
					if(WIFEXITED(status)){
						//java 返回非0表示出错
						if(problem::lang != judge_conf::LANG_JAVA 
								|| WEXITSTATUS(status) == EXIT_SUCCESS)
						{
							int result;
							if(problem::spj){
								//spj
								result = spj_compare_output(problem::input_file,
										problem::output_file,
										problem::spj_exe_file,
										problem::stdout_file_spj);
							}else
							{
								result = compare_output(problem::output_file_std,
										problem::output_file);
							}
							//记录结果
							if(result != judge_conf::OJ_AC){
								problem::result = result;
							}else{
								if(problem::result != judge_conf::OJ_PE)
									problem::result = result;
							}
						}else{
							LOG_BUG("abort quit code: %d\n",WEXITSTATUS(status));
							problem::result = judge_conf::OJ_RE_JAVA;
						}
						break;
					}

					// 收到信号
					// RE/TLE/OLE   超过sterlimit限制而结束
					// 且过滤掉被ptrace暂停的 SIGTRAP
					if(WIFSIGNALED(status) ||
							(WIFSTOPPED(status) && WSTOPSIG(status) != SIGTRAP))
					{
						int signo = 0;
						if(WIFSIGNALED(status))
							signo = WTERMSIG(status);
						else
							signo = WSTOPSIG(status);
						switch(signo){
							//TLE
							case SIGALRM:
								//LOG_BUG("ALRM");
							case SIGXCPU:
								//LOG_BUG("XCPU");
							case SIGVTALRM:
								//LOG_BUG("TALRM");
							case SIGKILL:
								//LOG_BUG("KILL");
								problem::result = judge_conf::OJ_TLE;
								break;
								//OLE
							case SIGXFSZ:
								problem::result = judge_conf::OJ_OLE;
								break;
							case SIGSEGV:
								problem::result = judge_conf::OJ_RE_SEGV;
								break;
							case SIGABRT:
								problem::result = judge_conf::OJ_RE_ABRT;
								break;
							default:
								problem::result = judge_conf::OJ_RE_UNKNOW;

						}
						//This is a debug
						//LOG_DEBUG("This is the file: %s\n",problem::input_file.c_str());
						//
						ptrace(PTRACE_KILL,userexe,NULL,NULL);
						break;
					}//end if

					//MLE
//					LOG_DEBUG("old memory: %d ,new memory: %d",problem::memory_usage,
//							rused.ru_minflt *(getpagesize()/judge_conf::KILO));
					int tempmemory = 0;

					if(problem::lang == judge_conf::LANG_JAVA)
					{
	  					tempmemory = rused.ru_minflt *(getpagesize()/judge_conf::KILO);
					}else{
						tempmemory = getmemory(userexe);

					}
					problem::memory_usage = Max(problem::memory_usage,tempmemory);
					if(problem::memory_usage > problem::memory_limit)
					{
						problem::result = judge_conf::OJ_MLE;
						ptrace(PTRACE_KILL,userexe,NULL,NULL);
						break;
					}

					//检查userexe的syscall
					if(ptrace(PTRACE_GETREGS,userexe,NULL,&regs) < 0){
						LOG_BUG("error ptrace ptrace_getregs");
						exit(judge_conf::EXIT_JUDGE);
					}
#ifdef __i386__
					syscall_id = regs.orig_eax;
#else
					syscall_id = regs.orig_rax;
#endif

					if(syscall_id > 0 && (!is_valid_syscall(problem::lang,syscall_id,userexe,regs)))
					{
						LOG_WARNING("error for syscall %d",syscall_id);
						problem::result = judge_conf::OJ_RF;
						ptrace(PTRACE_KILL,userexe,NULL,NULL);
						break;
					}

					if(ptrace(PTRACE_SYSCALL,userexe,NULL,NULL) < 0)
					{
						LOG_BUG("error ptrace ptrace syscall");
						exit(judge_conf::EXIT_JUDGE);
					}
				}//for(;;)
			}//else   userexe end

			problem::time_usage += rused.ru_utime.tv_sec*1000 +
				rused.ru_utime.tv_usec/1000;
			//非ac pe，没有继续的必要
			if(problem::result != judge_conf::OJ_AC &&
					problem::result != judge_conf::OJ_PE)
			{
				if(problem::result == judge_conf::OJ_TLE){
					problem::time_usage = problem::time_limit;
				}
				break;
			}

		}//if(isInfile())

	}//end while
	output_result(problem::result,problem::memory_usage,problem::time_usage);
	return 0;
}
