#include<bits/stdc++.h>
using namespace std;
int main(){
    int l,m,n;
    scanf("%d %d %d",&l,&n,&m);
    int right,a[50001],left=0;
    for(int i=1;i<=n;i++) scanf("%d",&a[i]);
    right=a[n+1]=l;
    while (right-left>1){
        int cot=0,now=0,mid;
        mid=(left+right)/2;
        for(int i=1;i<=n;i++){
            if (a[i]-a[now]<mid){
                cot++;
                if (cot>m){
                    right=mid;
                    break;
                }
            }
            else now=i;
        }
        if (cot<=m) left=mid;
    }
    if (m!=0) printf("%d",left);
    else printf("%d",l);
    system("pause");
    return 0;
}