#include<iostream>
#include<algorithm>
#include<string.h>
using namespace std;

int h[100005],s[100005];

int main()
{
	int n,i,j;
	long long ans;
	while(cin>>n)
	{
		ans=1;j=0;
		for(i=0;i<n;i++)
			cin>>s[i];
		for(i=0;i<n;i++)
			cin>>h[i];
		sort(s,s+n);
		sort(h,h+n);
		for(i=0;i<n;i++)
		{
			if(h[i]<s[i]) {ans=0;break;}
			for(;j<n;j++)
			{
				if(h[i]<s[j]) break;
			}
			ans*=(j-i);
			ans%=1000000007;
			j--;
		}
		cout<<ans<<endl;
	}
	return 0;
}