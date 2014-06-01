#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
 
const int MAXN = 10010;
const int MAXM = 100010;
const int INF = 0x3f3f3f3f;
struct Edge
{
    int to,w;
    int next;
}edge[MAXM];
int head[MAXN],tot;
void init()
{
    tot = 0;
    memset(head,-1,sizeof(head));
}
void addedge(int u,int v,int w)
{
    edge[tot].to = v;
    edge[tot].w = w;
    edge[tot].next = head[u];
    head[u] = tot++;
}
int dp1[MAXN];
int dp2[MAXN];
int n;
int a[MAXM];
void dfs(int u)
{
    if(u == n-1)
    {
        dp1[u] = dp2[u] = 0;
        return;
    }
    dp1[u] = dp2[u] = INF;
    int sz = 0;
    int smin = -1;
    for(int i = head[u];i != -1;i = edge[i].next)
    {
        int v = edge[i].to;
        dp1[u] = min(dp1[u],dp1[v] + edge[i].w);
        if(smin == -1)smin = dp2[v] + edge[i].w;
        else
            smin = min(smin,dp2[v] + edge[i].w);
        a[sz++] = dp1[v] + edge[i].w;
    }
    a[sz] = INF;
    for(int i = sz-2;i >= 0;i--)
        a[i] = min(a[i],a[i+1]);
    int tmp = INF;
    dp2[u] = -1;
    //printf("*** %d  %d\n",u,smin);
    if(smin !=-1)dp2[u] = smin;
    int tt = 0;
    for(int i = head[u];i != -1;i = edge[i].next)
    {
        int v = edge[i].to;
        tt++;
        if(dp2[u] == -1)
            dp2[u] = min(tmp,a[tt]);
        else dp2[u] = max(dp2[u],min(tmp,a[tt]));
        //if(dp2[u] == -1)
        //  dp2[u] = dp2[v] + edge[i].w;
        //else dp2[u] = max(dp2[u],dp2[v] + edge[i].w);
        tmp = min(tmp,dp1[v] + edge[i].w);
    }
    if(dp2[u] == -1)dp2[u] = INF;
}
 
vector<int>vec[MAXN];
int in[MAXN];
int b[MAXN];
int cnt;
void get()
{
    queue<int>q;
    for(int i = 0;i < n;i++)
        if(in[i] == 0)
        {
            q.push(i);
        }
    while(!q.empty())
    {
        int u = q.front();
        q.pop();
        b[cnt++] = u;
        int sz = vec[u].size();
        for(int i = 0;i < sz;i++)
        {
            int v = vec[u][i];
            in[v]--;
            if(in[v] == 0)
                q.push(v);
        }
    }
 
}
 
int main()
{
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    int T;
    int m;
    scanf("%d",&T);
    while(T--)
    {
        scanf("%d%d",&n,&m);
        init();
        int u,v,w;
        for(int i = 0;i < n;i++)
        {
            in[i] = 0;
            vec[i].clear();
        }
        while(m--)
        {
            scanf("%d%d%d",&u,&v,&w);
            addedge(u,v,w);
            vec[v].push_back(u);
            in[u]++;
        }
        cnt = 0;
        get();
        for(int i = 0;i < n;i++)
        {
            dfs(b[i]);
    //      printf("%d  %d  %d\n",b[i],dp1[b[i]],dp2[b[i]]);
        }
        if(dp2[0] >= INF)printf("-1\n");
        else printf("%d\n",dp2[0]);
    }
    return 0;
}