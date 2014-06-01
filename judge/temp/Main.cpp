/*
* this code is made by 19891101
* Problem: 1083
* Verdict: Dangerous Code
* Submission Date: 2014-05-31 01:00:17
* Time: -9 MS
* Memory: 15556 KB
*/
#include <iostream>
#include <cstdio>
#include <string.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <map>
 
 
#define max(x,y) ((x)>(y)?(x):(y))
#define min(x,y) ((x)<(y)?(x):(y))
#define abs(x) ((x)>=0?(x):-(x))
#define i64 long long
#define u32 unsigned int
#define u64 unsigned long long
#define clr(x,y) memset(x,y,sizeof(x))
#define CLR(x) x.clear()
#define ph(x) push(x)
#define pb(x) push_back(x)
#define Len(x) x.length()
#define SZ(x) x.size()
#define PI acos(-1.0)
#define sqr(x) ((x)*(x))
#define MP(x,y) make_pair(x,y)
#define EPS 1e-7
 
 
#define FOR0(i,x) for(i=0;i<x;i++)
#define FOR1(i,x) for(i=1;i<=x;i++)
#define FOR(i,a,b) for(i=a;i<=b;i++)
#define FORL0(i,a) for(i=a;i>=0;i--)
#define FORL1(i,a) for(i=a;i>=1;i--)
#define FORL(i,a,b)for(i=a;i>=b;i--)
 
 
#define rush() int CC;for(scanf("%d",&CC);CC--;)
#define Rush(n)  while(scanf("%d",&n)!=-1)
using namespace std;
 
 
void RD(int &x){scanf("%d",&x);}
void RD(i64 &x){scanf("%lld",&x);}
void RD(u64 &x){scanf("%llu",&x);}
void RD(u32 &x){scanf("%u",&x);}
void RD(double &x){scanf("%lf",&x);}
void RD(int &x,int &y){scanf("%d%d",&x,&y);}
void RD(i64 &x,i64 &y){scanf("%lld%lld",&x,&y);}
void RD(u32 &x,u32 &y){scanf("%u%u",&x,&y);}
void RD(double &x,double &y){scanf("%lf%lf",&x,&y);}
void RD(int &x,int &y,int &z){scanf("%d%d%d",&x,&y,&z);}
void RD(i64 &x,i64 &y,i64 &z){scanf("%lld%lld%lld",&x,&y,&z);}
void RD(u32 &x,u32 &y,u32 &z){scanf("%u%u%u",&x,&y,&z);}
void RD(double &x,double &y,double &z){scanf("%lf%lf%lf",&x,&y,&z);}
void RD(char &x){x=getchar();}
void RD(char *s){scanf("%s",s);}
void RD(string &s){cin>>s;}
 
 
void PR(int x) {printf("%d\n",x);}
void PR(int x,int y) {printf("%d %d\n",x,y);}
void PR(i64 x) {printf("%lld\n",x);}
void PR(u32 x) {printf("%u\n",x);}
void PR(u64 x) {printf("%llu\n",x);}
void PR(double x) {printf("%.4lf\n",x);}
void PR(char x) {printf("%c\n",x);}
void PR(char *x) {printf("%s\n",x);}
void PR(string x) {cout<<x<<endl;}
 
void upMin(int &x,int y) {if(x>y) x=y;}
void upMin(i64 &x,i64 y) {if(x>y) x=y;}
void upMin(double &x,double y) {if(x>y) x=y;}
void upMax(int &x,int y) {if(x<y) x=y;}
void upMax(i64 &x,i64 y) {if(x<y) x=y;}
void upMax(double &x,double y) {if(x<y) x=y;}
 
 
 
 
 
int sgn(double x)
{
    if(x>1e-6) return 1;
    if(x<-1e-6) return -1;
    return 0;
}
 
i64 Gcd(i64 x,i64 y)
{
    if(y==0) return x;
    return Gcd(y,x%y);
}
 
i64 Lcm(i64 x,i64 y)
{
    return x/Gcd(x,y)*y;
}
 
i64 eular(i64 n)
{
    i64 ans=n,i;
    for(i=2;i*i<=n;i++) if(n%i==0)
    {
        ans-=ans/i;
        while(n%i==0) n/=i;
    }
    if(n>1) ans-=ans/n;
    return ans;
}
 
 
i64 exGcd(i64 a,i64 b,i64 &x,i64 &y)
{
    if(b==0)
    {
        x=1;y=0;
        return a;
    }
    i64 temp=exGcd(b,a%b,x,y);
    i64 t=x;
    x=y;
    y=t-a/b*y;
    return temp;
}
 
i64 gcdReverse(i64 a,i64 b)
{
    i64 x,y;
    exGcd(a,b,x,y);
    x%=b;
    if(x<0) x+=b;
    return x;
}
 
i64 myPow(i64 a,i64 b)
{
    i64 ans=1;
    while(b)
    {
        if(b&1) ans=ans*a;
        a=a*a;
        b>>=1;
    }
    return ans;
}
 
i64 myPow(i64 a,i64 b,i64 mod)
{
    a%=mod;
    i64 ans=1;
    while(b)
    {
        if(b&1) ans=ans*a%mod;
        a=a*a%mod;
        b>>=1;
    }
    return ans;
}
 
 
int getInt()
{
    int x=0;
    char c=getchar();
    while(!isdigit(c)) c=getchar();
    while(isdigit(c))
    {
        x=x*10+c-'0';
        c=getchar();
    }
    return x;
}
 
i64 getSqrt(i64 t)
{
    i64 p=sqrt(1.0*t);
    while(p*p>t) p--;
    while((p+1)<=t/(p+1)) p++;
    return p;
}
 
const int mod=1000000007;
const i64 inf=(((i64)1)<<60)+5;
const double dinf=1000000000;
const int INF=1000000005;
const int N=100005;
 
/****************************/
 
 
 
/*****************************/
 
 
vector<pair<int,int> > g1[N],g2[N];
int n,m;
i64 f[N],d[N],dis[N];
int inq[N];
 
int Q[N];
 
int main()
{
    rush()
    {
        RD(n,m);
        int i;
        FOR0(i,n) g1[i].clear(),g2[i].clear(),d[i]=0;
        clr(d,0);
        FOR1(i,m)
        {
            int x,y,w;
            RD(x,y,w);
            g1[x].pb(MP(y,w));
            g2[y].pb(MP(x,w));
            d[y]++;
        }
        int head=0,tail=0;
        Q[tail++]=0;
        while(head<tail)
        {
            int u=Q[head++];
            FOR0(i,SZ(g1[u]))
            {
                int v=g1[u][i].first;
                if(--d[v]==0) Q[tail++]=v;
            }
        }
        queue<int> q;
        q.push(n-1);
        FOR0(i,n) dis[i]=inf,inq[i]=0;
        dis[n-1]=0;
        while(!q.empty())
        {
            int u=q.front();
            q.pop();
 
            inq[u]=0;
 
            FOR0(i,SZ(g2[u]))
            {
                int v=g2[u][i].first;
                int w=g2[u][i].second;
                if(dis[v]>dis[u]+w)
                {
                    dis[v]=dis[u]+w;
                    if(!inq[v]) q.push(v),inq[v]=1;
                }
            }
        }
        FOR0(i,n) f[i]=inf;
        f[n-1]=0;
        for(i=tail-2;i>=0;i--)
        {
            int u=Q[i];
            i64 Min1=inf,Min2=inf;
            int j;
            FOR0(j,SZ(g1[u]))
            {
                int v=g1[u][j].first;
                int w=g1[u][j].second;
                upMin(f[u],f[v]+w);
                i64 tmp=dis[v]+w;
                if(tmp<Min1) Min2=Min1,Min1=tmp;
                else if(tmp<Min2) Min2=tmp;
            }
            upMax(f[u],Min2);
        }
        if(f[0]>=inf) puts("-1");
        else PR(f[0]);
    }
}
 
/*
3
3 2
0 1 10
0 1 20
1 2 30
1 2 100
5 6
0 1 1
1 2 1
2 4 1
1 4 3
0 3 2
3 4 1
3 4
0 1 1
0 1 2
1 2 3
1 2 4
*/