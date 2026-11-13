#include<bits/stdc++.h>
using namespace std;
void BubbleSort(int n,int a[])
{
    int i,j;
    for(i=1;i<=n;i++)
        for(j=1;j<=n-i;j++)
            if (a[j]>a[j+1]) swap(a[j],a[j+1]);
}
int main(){
    int a[10001],n,i;
    cin>>n;
    for(i=1;i<=n;i++) cin>>a[i];
    BubbleSort(n,a);
    for(i=1;i<=n;i++) cout<<a[i]<<" ";
    system("pause");
    return 0;
}