#include<bits/stdc++.h>
using namespace std;
int minstep,ss;
void dfs(int v,int step) {
    ss++;
    if (step>minstep) return;
    if (v%32768==0) {
        minstep=min(minstep,step);
        return;
    }
    dfs(2*v,step+1);
    dfs(v+1,step+1);
}
int maxpow(int v) {
    int p=15;
    while (p) {
        int exp=pow(2,p);
        if (v%exp==0) return p;
        else p--;
    }
    return 0;
}
int main(){
    int n;
    cin>>n;
    for (int i=0;i<n;i++) {
        int v,mp;
        cin>>v;
        mp=v%16;
    }
    system("pause");
    return 0;
}