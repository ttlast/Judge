#!/usr/bin/python
#-*-coding:utf-8 -*-

import urllib
import cookielib 
import urllib2
import random

print "Content-Type: text/plain"

def make_cookie_header(cookie):
    ret = ""
    for val in cookie.values():
        ret+="%s=%s; "%(val.key, val.value)
    return ret

def submit_oj(problemid,code):
    login_url = r'http://172.22.27.122:8080/doLogin'
    login_data = urllib.urlencode({'username' : 'admin','password' : 'root',})
    cj = cookielib.CookieJar()
    try:
    	opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
    	opener.addheaders = [('User-agent','Opera/9.23')]
    	urllib2.install_opener(opener)
    	req = urllib2.Request(login_url,login_data)
    	conn = urllib2.urlopen(req)
    except:
    	print "login wa"
	
    submit_url = r'http://172.22.27.122:8080/submit?pID=1000'
    submit_data = urllib.urlencode(
          {
           'problemid' : problemid,
           'language' : 2,
           'sourcecode' : code[random.randint(0,3)],
		   #'sourcecode' : code[3],
          })
    try:
        req2 = urllib2.Request(submit_url,submit_data)
        urllib2.urlopen(req2)
        #print urllib2.urlopen(req2).read()
    except urllib2.HTTPError,error:
        print error.read()
        
print "Content-Type: text/plain"
problemid=1000
code = [r'''#include <stdio.h>
int main(){
	int a,b;
	scanf("%d%d",&a,&b);
	printf("%d\n",a+b);
	return 1;
}
''',\
r'''#include <stdio.h>
int main(){
	int a,b;
	scanf("%d%d",&a,&b);
	printf("%d \n",a+b);
	return 1;
}
''',\
r'''#include <stdio.h>
int main(){
	int a,b;
	scanf("%d%d",&a,&b);
	printf("%d\n",a-b);
	return 1;
}''',\
r'''#include <stdio.h>
int main(){
	while(1);
	int a,b;
	scanf("%d%d",&a,&b);
	printf("%d\n",a-b);
	return 1;
}''']

for i in xrange(1,80):
    submit_oj(problemid,code)
#result = submit_oj(problemid,code)
#print result
