/*
* this code is made by Blaziken
* Problem: 1096
* Verdict: Dangerous Code
* Submission Date: 2014-05-30 19:11:49
* Time: -5 MS
* Memory: 12804 KB
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int num[1000050],numm[1000050];
 
int cmp(const void *a,const void *b){
    return *(int *)a-*(int *)b;
}
 
int main(void){
    int n,i;
    while(~scanf("%d",&n)){
       for(i=0;i<n;i++)
          scanf("%d",&num[i]);
       qsort(num,n,sizeof(num[0]),cmp);
       for(i=0;i<n-1;i++)
          scanf("%d",&numm[i]);
       qsort(numm,n-1,sizeof(numm[0]),cmp);
       for(i=0;i<n;i++){
          if(num[i]!=numm[i]){
             printf("%d\n",num[i]);
             break;
          }
       }
    }
    return 0;
}