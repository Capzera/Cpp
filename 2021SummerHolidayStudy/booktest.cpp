#include<bits/stdc++.h>
using namespace std;
int a[1000002],n;
bool bo[1000002];
long long ans=0;
long long get(){
    int sum=0,tmp=0;
    for(int i=1;i<=n;i++){
        if(bo[i])
        {
            sum+=a[i];
            if (sum<=0) 
            {
                tmp+=abs(sum)+1;
                sum=1;
            }
        }
        else
        {
            sum+=a[i];
            if (sum>=0)
            {
                tmp+=sum+1;
                sum=-1;
            }
        }
    }
    return tmp;
}
int main(){
    cin>>n;
    for(int i=1;i<=n;i++)cin>>a[i];
    for(int i=1;i<=n;i++)bo[i]=i%2;
    ans=get();
    for(int i=1;i<=n;i++)bo[i]=1-i%2;
    ans=min(ans,get());
    printf("%d",ans);
    system("pause");
    return 0;
}