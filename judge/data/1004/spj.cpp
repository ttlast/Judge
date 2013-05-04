#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
using namespace std;

int a[105], n, has[1005] = {0};

bool judge () {
	int i, j, k;
	for (i = 1; i <= n; i++) {
		for (j = 1; j <= n; j++) if (i != j) {
			for (k = 1; k <= n; k++) if (k != i && k != j) {
				if (a[i] == a[j] + a[k])
					return false;
			}
		}
	}
	return true;
}

bool check (int x, int y, int z) {
	if (has[x] <= 0) return false;
	--has[x];
	if (has[y] <= 0) return false;
	--has[y];
	if (has[z] <= 0) return false;
	return true;
}

int main(int argc,char *argv[])
{
	//FILE *test_fd = fopen("tt.txt","a");
	FILE *input_fd = fopen(argv[1],"r");
	FILE *output_fd = fopen(argv[2],"r");
	//fprintf(test_fd, "%s\n", argv[1]);
	//fclose(test_fd);
	if (fscanf(input_fd, "%d" ,&n) == EOF) {
		puts("WA");
		goto end;
	}
	int x, y ,z;
	for (int i = 1; i <= n; i++) {
		if (fscanf(input_fd, "%d", a+i) == EOF) {
			puts("WA");
			goto end;
		}
		++has[a[i]];
	}
	if (fscanf(output_fd, "%d", &x) == EOF) {
		puts("WA");
		goto end;
	}
	if (x == -1) {
		if (judge()) puts("AC"); 
		else puts("WA");
	} else {
		if (fscanf(output_fd, "%d", &y) == EOF) {
			puts("WA");
			goto end;
		}
		if (fscanf(output_fd, "%d", &z) == EOF) {
			puts("WA");
			goto end;
		}
		if (a[x] == a[y] + a[z] && check(a[x], a[y], a[z])) puts("AC");
		else puts("WA");
	}
	end:;
	//fclose(test_fd);
	if(input_fd) fclose(input_fd);
	if(output_fd) fclose(output_fd);
	return 0;
}
