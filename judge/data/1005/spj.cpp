#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
using namespace std;

int main(int argc,char *argv[])
{
	FILE *input_fd = fopen(argv[1],"r");
	FILE *output_fd = fopen(argv[2],"r");
	int n, a[55], i, j;
	fscanf(input_fd, "%d" ,&n);
	if (n < 3) {
		if (fscanf(output_fd, "%d", &i) == EOF) {
			puts("WA");
			goto end;
		}
		if (i == -1) puts("AC");
		else puts("WA");
	} else {
		for (i = 1; i <= n; i++) {
			if (fscanf(output_fd, "%d", a+i) == EOF) {
				puts("WA");
				goto end;
			}
		}
		for (i = 1; i < n; i++) {
			for (j = i; j < n; j++) {
				if (a[j] > a[j+1]) {
					int tp = a[j];
					a[j] = a[j+1];
					a[j+1] = tp;
				}
			}
		}
		for (i = 1; i < n; i++)
			if (a[i] > a[i+1])
				break;
		if (i < n) puts("AC");
		else puts("WA");
	}
	end:;
	fclose(input_fd);
	fclose(output_fd);
	return 0;
}
