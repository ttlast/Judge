
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <con>
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
