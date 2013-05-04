/*
 * 	
 *  syscall 调用的可用次数
 *	-1	禁止调用
 *	1 ->  可以调用次数
 *	0	不限制调用次数
 */

#ifndef __OKCALL__
#define __OKCALL__

#include <sys/syscall.h>
#include "judge.h"


short ok_table[1024] = {0};

#if __WORDSIZE == 32

int ok_c[512] = 
{
	243,1,
	SYS_time, 1,
	SYS_access, -1,
	SYS_brk, -1,
	SYS_close, -1,
	SYS_execve, 1,
	SYS_exit_group, -1,
	SYS_fstat64, -1,
	SYS_futex, -1,
	SYS_gettimeofday, -1,
	SYS_mmap2, -1,
	SYS_mprotect, -1,
	SYS_munmap, -1,
	SYS_lseek, -1,
	SYS_read, -1,
	SYS_uname, -1,
	SYS_write, -1,
	SYS_writev, -1,
	-1
};

int ok_cpp[512] = 
{
	270,-1,
	224,-1,	
	140,-1,
	91,-1,
	175,-1,
	195,-1,
	13,-1,
	5,-1,
	SYS_time, 1,
	SYS_access, -1,
	SYS_brk, -1,
	SYS_close, -1,
	SYS_execve, 1,
	SYS_exit_group, -1,
	SYS_fstat64, -1,
	SYS_futex, -1,
	SYS_gettimeofday, -1,
	SYS_mmap2, -1,
	SYS_mprotect, -1,
	SYS_munmap, -1,
	SYS_lseek, -1,
	SYS_read, -1,
	SYS_set_thread_area, -1,
	SYS_uname, -1,
	SYS_write, -1,
	SYS_writev, -1,
	-1
};

int ok_pas[512] = 
{
	SYS_time, 1,
	SYS_close, -1,
	SYS_execve, 1,
	SYS_exit_group, -1,
	SYS_futex, -1,
	SYS_gettimeofday, -1,
	SYS_ioctl, -1,
	SYS_mmap, -1,
	SYS_munmap, -1,
	SYS_lseek, -1,
	SYS_read, -1,
	SYS_readlink, -1,
	SYS_rt_sigaction, -1,
	SYS_ugetrlimit, -1,
	SYS_uname, -1,
	SYS_write, -1,
	SYS_writev, -1,
	-1
};

int ok_java[512] = 
{
	295,-1,
	SYS_time, 1,		
	SYS_access, -1,
	SYS_brk, -1,
	SYS_clone, -1,
	SYS_close, -1,
	SYS_execve, -1,
	SYS_exit, -1,
	SYS_exit_group, -1,
	SYS_fcntl64, -1,
	SYS_fstat64, -1,
	SYS_futex, -1,
	SYS_getdents64, -1,
	SYS_getegid32, -1,
	SYS_geteuid32, -1,
	SYS_getgid32, -1,
	SYS_getrlimit, -1,
	SYS_gettimeofday, -1,
	SYS_getuid32, -1,
	SYS_mmap, -1,
	SYS_mmap2, -1,
	SYS_mprotect, -1,
	SYS_munmap, -1,
	SYS_lseek, -1,
	SYS_open, -1,
	SYS_read, -1,
	SYS_readlink, -1,
	SYS_rt_sigaction, -1,
	SYS_rt_sigprocmask, -1,
	SYS_set_robust_list, -1,
	SYS_set_thread_area, -1,
	SYS_set_tid_address, -1,
	SYS_sigprocmask, -1,
	SYS_stat64, -1,
	SYS_ugetrlimit, -1,
	SYS_uname, -1,
	-1
};
#else
int ok_c[512] =
{
    SYS_arch_prctl,     -1,
    SYS_brk,            -1,
    SYS_close,          -1,
    SYS_execve,          1,
    SYS_exit_group,     -1,
    SYS_fstat,          -1,
    SYS_futex,          -1,
    SYS_gettimeofday,   -1,
    SYS_mmap,           -1,
    SYS_mremap,         -1,
    SYS_mprotect,       -1,
    SYS_munmap,         -1,
    SYS_lseek,          -1,
    SYS_read,           -1,
    SYS_set_thread_area,-1,
    SYS_uname,          -1,
    SYS_write,          -1,
    SYS_writev,         -1,
    -1
};

int ok_cpp[512] = 
{
    SYS_access,         -1,
    SYS_arch_prctl,     -1,
    SYS_brk,            -1,
    SYS_close,          -1,
    SYS_execve,          1,
    SYS_exit_group,     -1,
    SYS_fstat,          -1,
    SYS_futex,          -1,
    SYS_gettimeofday,   -1,
    SYS_mmap,           -1,
    SYS_mremap,         -1,
    SYS_mprotect,       -1,
    SYS_munmap,         -1,
    SYS_lseek,          -1,
    SYS_read,           -1,
    SYS_set_thread_area,-1,
    SYS_uname,          -1,
    SYS_write,          -1,
    SYS_writev,         -1,
    -1
};

int ok_pas[512] = 
{
    SYS_close,          -1,
    SYS_execve,         1,
    SYS_exit_group,     -1,
    SYS_futex,          -1,
    SYS_getrlimit,      -1,
    SYS_gettimeofday,   -1,
    SYS_ioctl,          -1,
    SYS_mmap,           -1,
    SYS_mremap,         -1,
    SYS_munmap,         -1,
    SYS_lseek,          -1,
    SYS_read,           -1,
    SYS_readlink,       -1,
    SYS_rt_sigaction,   -1,
    SYS_uname,          -1,
    SYS_write,          -1,
    SYS_writev,         -1,
    -1
};

int ok_java[512] = 
{
    SYS_access,         -1,
    SYS_arch_prctl,     -1,
    SYS_brk,            -1,
    SYS_clone,          -1,
    SYS_close,          -1,
    SYS_execve,         -1,
    SYS_exit_group,     -1,
    SYS_fstat,          -1,
    SYS_futex,          -1,
    SYS_getegid,        -1,
    SYS_geteuid,        -1,
    SYS_getgid,         -1,
    SYS_getrlimit,      -1,
    SYS_gettimeofday,   -1,
    SYS_getuid,         -1,
    SYS_mmap,           -1,
    SYS_mremap,         -1,
    SYS_mprotect,       -1,
    SYS_munmap,         -1,
    SYS_lseek,          -1,
    SYS_open,           -1,
    SYS_read,           -1,
    SYS_readlink,       -1,
    SYS_rt_sigaction,   -1,
    SYS_rt_sigprocmask, -1,
    SYS_set_robust_list,-1,
    SYS_set_tid_address,-1,
    SYS_stat,           -1,
    SYS_uname,          -1,
    SYS_write,          -1,
    SYS_writev,         -1,
    -1
};
#endif
//初始化，系统调用限制表格
//
void init_ok_table(int lang)
{
	int *p = NULL;
	switch(lang)
	{
		case judge_conf::LANG_C:
			p = ok_c;
			break;
		case judge_conf::LANG_CPP:
			p = ok_cpp;
			break;
		case judge_conf::LANG_JAVA:
			p = ok_java;
			break;
		default:
			///////LOG  bug！！！
			break;
	}
	memset(ok_table,0,sizeof(ok_table));
	for(int i = 0;p[i] >= 0;i += 2)
	{
		ok_table[p[i]] = p[i+1];
	}
}

static bool in_syscall = true;

bool is_valid_syscall(int lang,int syscall_id,pid_t child,user_regs_struct regs)
{
	in_syscall = !in_syscall;

	//被限制调用的函数，syscall_id = 0
	if(ok_table[syscall_id] == 0){
		long addr;
		if(syscall_id == SYS_open){
#if __WORDSIZE == 32
			addr = regs.ebx;
#else
			addr = regs.rdi;
#endif
			union u{unsigned long val;char chars[sizeof(long)];} data;
			unsigned long i = 0,j = 0,k = 0;
			char filename[1024];
			for(;;){
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
