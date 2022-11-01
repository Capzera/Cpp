#include<bits/stdc++.h>
using namespace std;
void CapSort(int n,int a[])
{
    int i,j;
    for(i=1;i<n;i++)
        for(j=i+1;j<=n;j++)
            if (a[i]>a[j]) swap(a[i],a[j]);
}
int main(){
    int a[10001],i,j,n;
    cin>>n;
    for(i=1;i<=n;i++)cin>>a[i];
    CapSort(n,a);
    for(i=1;i<=n;i++)cout<<a[i]<<" ";
    system("pause");
    return 0;
}