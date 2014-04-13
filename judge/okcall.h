/*
 * 	
 *  syscall 调用的可用次数
 *	0	禁止调用
 *	1 ->  可以调用次数
 *	-1	不限制调用次数
 */

#ifndef __OKCALL__
#define __OKCALL__

#include <sys/syscall.h>
#include <cstdio>
#include "judge.h"


short ok_table[1024] = {0};


//初始化，系统调用限制表格
//
bool init_ok_table()
{
	int scall, stimes;
	FILE* cfg = fopen("okcall.cfg","r");
	if (!cfg) return false;
	memset(ok_table,0,sizeof(ok_table));
	while (~fscanf(cfg,"%d := %d", &scall, &stimes))
	{
		ok_table[scall] = stimes;
	}
	return true;
}

static bool in_syscall = true;

bool is_valid_syscall(int lang,int syscall_id,pid_t child,user_regs_struct regs)
{
	in_syscall = !in_syscall;

	//被限制调用的函数，syscall_id = 0
	if(ok_table[syscall_id] == 0){
		long addr;
		if(syscall_id == SYS_open){
#if __i386__
			addr = regs.ebx;
#else
			addr = regs.rbi;
#endif
			union u{unsigned long val;char chars[sizeof(long)];} data;
			unsigned long i = 0,j = 0,k = 0;
			char filename[1024];
			while (true) {
				data.val = ptrace(PTRACE_PEEKDATA,child,addr+i,NULL);
				i += sizeof(long);
				for(j = 0;j < sizeof(long) && data.chars[j] > 0 && k < 256;j ++){
					filename[k++] = data.chars[j];
				}
				if(j < sizeof(long) && data.chars[j] == 0)
					break;
			}
			filename[k] = 0;
			if(strstr(filename,"/proc/") == filename)
				return true;
			LOG_NOTICE("syscall_id %d syscall open: %s",syscall_id,filename);
		}
		return false;
	}else if(ok_table[syscall_id] > 0){
		if(!in_syscall)
			ok_table[syscall_id] --;
	}
	return true;
}
#endif
