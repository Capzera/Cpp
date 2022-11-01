#include <iostream>
#include <algorithm>
#include <stdlib.h>
using namespace std;
const int N = 1e4 + 10;
const int inf = 0x3f3f3f3f;
int ufs[N];
struct Edge{
    int u;
    int v;
    int w;
}edges[N];
int n,m,j=0,res=0;
int find(int x){
    if (ufs[x]!=x) ufs[x]=find(ufs[x]);
    return ufs[x];
}
void kruskal(){
    sort(edges, edges + j, [](const struct Edge &x , const struct Edge &y){return x.w < y.w;});
    for (int i=0;i<j;i++){
        int u=edges[i].u;
        int v=edges[i].v;
        if (find(u)!=find(v)){
            int w=edges[i].w;
            ufs[find(u)]=find(v);
            res+=w;
        }
    }
}

int main() {
    cin>>n>>m;
    for (int i=1;i<=n;i++) ufs[i] = i;
    for (int i=0;i<m;i++) {
        int p,u,v,w;
        cin>>p>>u>>v>>w;
        if (p==1) {
            ufs[find(u)]=find(v);
            res+=w;
        }
        else edges[j++]={u,v,w};
    }
    kruskal();
    cout<<res<<endl;
    system("pause");
    return 0;
}