#include <stdio.h>
int main(){
   FILE* f=fopen("data.in","r");
  int a,b;
  fscanf(f,"%d%d",&a,&b);
  printf("%d",a+b);
  return 0;
}