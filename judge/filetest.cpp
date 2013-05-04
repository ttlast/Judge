#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
using namespace std;

int main(int argc,char *argv[])
{
	string file_std = "data/1007/lines3.out";
	string file_usr = "temp/lines3.out";
	FILE *fd_std = fopen(file_std.c_str(),"r");
	FILE *fd_usr = fopen(file_usr.c_str(),"r");
	if(!fd_std) printf("std cann't open");
	if(!fd_usr) printf("usr cann't open");
	if(fd_std) fclose(fd_std);
	if(fd_usr) fclose(fd_usr);
	 file_std = "data/1007/lines1.out";
	 file_usr = "temp/lines1.out";
	fd_std = fopen(file_std.c_str(),"r");
	fd_usr = fopen(file_usr.c_str(),"r");
	if(!fd_std) printf("std cann't open");
	if(!fd_usr) printf("usr cann't open");
	if(fd_std) fclose(fd_std);
	if(fd_usr) fclose(fd_usr);

	 file_std = "data/1007/lines4.out";
	 file_usr = "temp/lines4.out";
	fd_std = fopen(file_std.c_str(),"r");
	fd_usr = fopen(file_usr.c_str(),"r");
	if(!fd_std) printf("std cann't open");
	if(!fd_usr) printf("usr cann't open");
	if(fd_std) fclose(fd_std);
	if(fd_usr) fclose(fd_usr);

	 file_std = "data/1007/lines5.out";
	 file_usr = "temp/lines5.out";
	fd_std = fopen(file_std.c_str(),"r");
	fd_usr = fopen(file_usr.c_str(),"r");
	if(!fd_std) printf("std cann't open");
	if(!fd_usr) printf("usr cann't open");
	if(fd_std) fclose(fd_std);
	if(fd_usr) fclose(fd_usr);

	 file_std = "data/1007/lines6.out";
	 file_usr = "temp/lines6.out";
	fd_std = fopen(file_std.c_str(),"r");
	fd_usr = fopen(file_usr.c_str(),"r");
	if(!fd_std) printf("std cann't open");
	if(!fd_usr) printf("usr cann't open");
	if(fd_std) fclose(fd_std);
	if(fd_usr) fclose(fd_usr);
	
	 file_std = "data/1007/lines7.out";
	 file_usr = "temp/lines7.out";
	fd_std = fopen(file_std.c_str(),"r");
	fd_usr = fopen(file_usr.c_str(),"r");
	if(!fd_std) printf("std cann't open");
	if(!fd_usr) printf("usr cann't open");
	if(fd_std) fclose(fd_std);
	if(fd_usr) fclose(fd_usr);

	 file_std = "data/1007/lines8.out";
	 file_usr = "temp/lines8.out";
	fd_std = fopen(file_std.c_str(),"r");
	fd_usr = fopen(file_usr.c_str(),"r");
	if(!fd_std) printf("std cann't open");
	if(!fd_usr) printf("usr cann't open");
	if(fd_std) fclose(fd_std);
	if(fd_usr) fclose(fd_usr);


	return 0;
}
