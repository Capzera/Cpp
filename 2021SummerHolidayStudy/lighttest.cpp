#include<bits/stdc++.h>
using namespace std;
int h,n,m,l[110],r[110],ans;
char a[110][1010];
void dfs(int i,int lr,int now){
    if (i==h){
        if(!lr) now+=r[i];
        else now+=m-l[i]+1;
        ans=min(ans,now);
        return;
    }
    if (!lr) dfs(i-1,0,now+2*r[i]+1);
    else dfs(i-1,1,now+2*(m-l[i]+1)+1);
    dfs(i-1,lr^1,now+m+2);
}
int main(){
    cin>>n>>m;
    h=n;
    for(int i=1;i<=n;i++){
        scanf("%s",a[i]);
        l[i]=m+1;
        r[i]=0;
        for(int j=0;j<m+2;j++){
            if(a[i][j]=='1'){
                l[i]=min(l[i],j);
                r[i]=max(r[i],j);
                h=min(h,i);
            }
        }
    }
    ans=1010*110*10;
    dfs(n,0,0);
    printf("%d",ans);
    system("pause");
    return 0;
}