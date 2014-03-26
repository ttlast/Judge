#!/usr/bin/python
#-*-coding:utf-8-*-
#
#This is a test for daemon.py of 

import os
import subprocess

tmpdir = "./test/temp"
langf = {1:"Main.c",2:"Main.cpp",3:"Main.java"}
def makefile(lang,val):
	try:
		sfile = tmpdir + "/" + langf[lang]
		fd = open(sfile,"w")
		fd.write(val)
		fd.close()
	except:
		exit(1)

class judge:
	'''
	
	'''
	def __init__(self,lang,datadir = "./test/data",tmpdir = "./test/temp",\
			timelimit = 1000,memlimit = 65535,outlimit = 8192,spj = False,tc = False):
		self.lang = lang
		self.datadir = datadir
		self.tmpdir = tmpdir
		self.timelimit = timelimit
		self.memlimit = memlimit
		self.outlimit = outlimit
		self.spj = spj
		self.tc = tc
	
	def run(self):
		#try:
			#os.chdir("./test")
		self.result,self.mem,self.time = (0,0,0)
		if self.spj and self.tc:
			p = subprocess.Popen("./test/test -l "+str(self.lang)+" -D "+self.datadir\
					+" -d "+self.tmpdir+" -t "+str(self.timelimit)+" -m "+str(self.memlimit)+" -o "+str(self.outlimit) + " -S dd -T",shell=True,stdout=subprocess.PIPE)
		elif self.spj:
			p = subprocess.Popen("./test/test -l "+str(self.lang)+" -D "+self.datadir\
					+" -d "+self.tmpdir+" -t "+str(self.timelimit)+" -m "+str(self.memlimit)+" -o "+str(self.outlimit) + " -S dd",shell=True,stdout=subprocess.PIPE)
		elif self.tc:
			p = subprocess.Popen("./test/test -l "+str(self.lang)+" -D "+self.datadir\
					+" -d "+self.tmpdir+" -t "+str(self.timelimit)+" -m "+str(self.memlimit)+" -o "+str(self.outlimit) + " -T",shell=True,stdout=subprocess.PIPE)
		else:
			p = subprocess.Popen("./test/test -l "+str(self.lang)+" -D "+self.datadir\
					+" -d "+self.tmpdir+" -t "+str(self.timelimit)+" -m "+str(self.memlimit)+" -o "+str(self.outlimit),shell=True,stdout=subprocess.PIPE)
		for l in p.stdout:
			(self.result,self.mem,self.time) = l.split()
		#except :
		#	exit(1)
		print (self.result,self.mem,self.time)
		

val = r'''
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <sys/types.h>
using namespace std;

class myclass{
	int a,b;
	
public:
	int scan(){
		char buf[100];
		if(!fgets(buf,100,stdin))
			return -1;
		sscanf(buf,"%d%d",&a,&b);
		//if(scanf("%d%d",&a,&b) == EOF)
		//	return -1;
		return 0;
	}
	int add(){
		return a+b;
	}
};

int main()
{
	int i = time(0);
	//pid_t pp = getppid();
	
	myclass mc;
	mc.scan();
	printf("%d\n",mc.add());
	return 0;
}
'''
makefile(2,val)
d = judge(lang=2)
d.run();

