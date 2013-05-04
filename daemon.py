#!/usr/bin/python
#coding=utf-8
#
#Author: TT_last
#
#This is a daemon for linux judge
#
#
#
import codecs
import sys, os, time, atexit
import subprocess
from pymongo import Connection
from signal import SIGTERM

#This is a daemon module
class Daemon:
	'''
	A daemon for gzhu judge
	'''
	def __init__(self,pidfile,stdin='/dev/null',stdout='/dev/null',stderr='/dev/null'):
		self.stdin = stdin
		self.stdout = stdout
		self.stderr = stderr
		self.pidfile = pidfile

	def daemonize(self):
		'''
		'''
		try:
			pid = os.fork()
			if pid > 0:
				sys.exit(0)
		except OSError,e:
			sys.stderr.write("fork #1 failed %d (%s)\n"%(e.errno,e.strerror))
			sys.exit(1)

		os.chdir("./judge");
		os.setsid()
		#os.umask(0)

		try:
			pid = os.fork()
			if pid > 0:
				sys.exit(0)
		except OSError, e:
			sys.stderr.write("fork #2 failed: %d (%s)\n"%(e.errno,e.strerror))
			sys.exit(1)

		#redirect standard io
		sys.stdout.flush()
		sys.stderr.flush()
		sin = file(self.stdin,"r")
		sout = file(self.stdout,"a+")
		serr = file(self.stderr,"a+",0)
		os.dup2(sin.fileno(),sys.stdin.fileno())
		os.dup2(sout.fileno(),sys.stdout.fileno())
		os.dup2(serr.fileno(),sys.stderr.fileno())

		#write pidfile
		atexit.register(self.delpid)
		pid = str(os.getpid())

		file(self.pidfile,"w+").write("%s\n" % pid);

	def delpid(self):
		os.remove(self.pidfile)

	def start(self):
		'''
		start the daemon
		'''
		try:
			pf = file(self.pidfile,'r')
			pid = int(pf.read().strip())
			pf.close()
		except IOError:
			pid = None

		if pid:
			message = "pidfile %s alread exist.Daemon already running?\n"
			sys.stderr.write(message % self.pidfile)
			sys.exit(1)


		self.daemonize()
		self.run()

	def stop(self):
		try:
			pf = file(self.pidfile,'r')
			pid = int(pf.read().strip())
			pf.close()
		except IOError:
			pid = None

		if not pid:
			message = "pidfile %s does not exist.Daemon not running?\n"
			sys.stderr.write(message % self.pidfile)
			return 

		try:
			while True:
				os.kill(pid,SIGTERM)
				time.sleep(0.1)
		except OSError, err:
			err = str(err)
			if err.find("No such process") > 0:
				if os.path.exists(self.pidfile):
					os.remove(self.pidfile)
			else:
				sys.exit(1)

	def restart(self):
		self.stop()
		self.start()

	def run(slef):
		pass

#This is the class for judge
#
cefile = "./temp/ce.txt"
dadir = "./data"
tmdir = "./temp"
langf = {1:"Main.c",2:"Main.cpp",3:"Main.java"}
def makefile(lang,val):
	try:
		sfile = tmdir + "/" + langf[lang]
#		print sfile
		fd = codecs.open(sfile,"wb",'utf-8')
#		print sfile
		fd.write(val)
		fd.close()
		return True
	except:
		return False



class judge:
	'''

	'''
	def __init__(self,lang,datadir = "./data",tmpdir = "./temp",\
			timelimit = 1000,memlimit = 65535,outlimit = 8192,spj = False,tc = False):
		self.lang = lang
		self.datadir = datadir
		self.tmpdir = tmpdir
		self.timelimit = timelimit
		self.memlimit = memlimit
		self.outlimit = outlimit
		self.spj = spj
		self.tc = tc

	def setlimit(self,timelimit = 1000,memlimit = 65535,outlimit = 8192):
		self.timelimit = timelimit
		self.memlimit = memlimit
		self.outlimit = outlimit

	def run(self):
		try:
			#os.chdir("./Judge")
			self.result,self.mem,self.time = (0,0,0)
			if self.spj and self.tc:
				p = subprocess.Popen("./Judge -l "+str(self.lang)+" -D "+self.datadir\
						+" -d "+self.tmpdir+" -t "+str(self.timelimit)+" -m "+str(self.memlimit)+" -o "+str(self.outlimit) + " -S dd -T",shell=True,stdout=subprocess.PIPE)
			elif self.spj:
				p = subprocess.Popen("./Judge  -l "+str(self.lang)+" -D "+self.datadir\
						+" -d "+self.tmpdir+" -t "+str(self.timelimit)+" -m "+str(self.memlimit)+" -o "+str(self.outlimit) + " -S dd",shell=True,stdout=subprocess.PIPE)
			elif self.tc:
				p = subprocess.Popen("./Judge -l "+str(self.lang)+" -D "+self.datadir\
						+" -d "+self.tmpdir+" -t "+str(self.timelimit)+" -m "+str(self.memlimit)+" -o "+str(self.outlimit) + " -T",shell=True,stdout=subprocess.PIPE)
			else:
				p = subprocess.Popen("./Judge -l "+str(self.lang)+" -D "+self.datadir\
						+" -d "+self.tmpdir+" -t "+str(self.timelimit)+" -m "+str(self.memlimit)+" -o "+str(self.outlimit),shell=True,stdout=subprocess.PIPE)
			for l in p.stdout:
				(self.result,self.mem,self.time) = l.split()
			self.result = int(self.result)
			self.mem = int(self.mem)
			self.time = int(self.time)
		except :
			exit(1)
#		print (self.result,self.mem,self.time)

OJ_WAIT = 0
OJ_RUN = 1
OJ_AC = 2
OJ_PE = 3
OJ_TLE = 4
OJ_MLE = 5
OJ_WA = 6
OJ_OLE = 7
OJ_CE = 8

#this is the judgeDaemon
class JudgeDaemon(Daemon):
	def run(self):
		while True:
			try:
				con = Connection("127.0.0.1")
				db = con.gzhu_db
				users = db.users
				problems = db.problems
				solutions = db.solutions
				one_solution = solutions.find_one({'result':OJ_WAIT})
				if one_solution != None:
					user = users.find_one({'name':one_solution['userName']})
					#print one_solution["language"],one_solution["code"]
					if makefile(int(one_solution["language"]),one_solution["code"]):
						solutions.update({"_id":one_solution["_id"]},{"$set":{"result":OJ_RUN}})
						#print one_solution["language"],one_solution["code"]
						#print dadir + "/" + str(one_solution['problemID'])
						one_problem = problems.find_one({'problemID':int(one_solution['problemID'])})
						if int(one_problem['spj']) == 0:
							gzhujudge = judge(one_solution["language"],datadir = dadir +"/" + str(one_solution['problemID']));
						else:
							gzhujudge = judge(one_solution["language"],datadir = dadir +"/" + str(one_solution['problemID']),spj=True);
						gzhujudge.setlimit(int(one_problem['timeLimit']),int(one_problem['memoryLimit']))	
						gzhujudge.run();
						if gzhujudge.result == OJ_CE:
							ce_file = open(cefile)
							try:
								all_ce_text = ce_file.read()
								solutions.update({"_id":one_solution["_id"]},{"$set":{"CE":all_ce_text}})
							finally:
								ce_file.close()	
						if gzhujudge.result == OJ_AC: #AC
							problems.update({"problemID":int(one_solution['problemID'])},{"$inc":{"AC":1}})
							is_ac = solutions.find_one({'problemID':int(one_solution['problemID']),'userName':one_solution['userName'],'result':OJ_AC})
							if is_ac == None:
								users.update({'name':user['name']},{"$inc":{"solved":1}})
						#users.update({'name':user['name']},{"$inc":{"submit":1}})
						solutions.update({"_id":one_solution["_id"]},{"$set":{"result":gzhujudge.result,"time":gzhujudge.time,"memory":gzhujudge.mem}})
				else:
					pass
			except:
				pass
			time.sleep(1)



if __name__ == "__main__":
	daemon = JudgeDaemon('/home/ttlast/OJ/dtest/daemon.pid',stdout="/dev/stdout") #绝对路径
	if len(sys.argv) == 2:
		if 'start' == sys.argv[1]:
			daemon.start()
		elif 'stop' == sys.argv[1]:
			daemon.stop()
		elif 'restart' == sys.argv[1]:
			daemon.restart()
		else:
			print "Unknown command"
			sys.exit(2)
		sys.exit(0)
	else:
		print "usage: %s start|stop|restart" % sys.argv[0]
		sys.exit(2)

