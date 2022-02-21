#include<bits/stdc++.h>
using namespace std;
void QSort(int l,int r,int a[])
{
    int i=l,j=r,mid=(l+r)/2;
    while(i<=j)
    {
        while(a[i]<a[mid]) i++;//将左半边都置为较小值
        while(a[j]>a[mid]) j--;//将右半边都置为较大值
        if (i<=j) //发现右边应该是较小值左边应该是较大值
        {
            swap(a[i],a[j]);//交换
            i++;
            j--;
        }
    }
    if (l<j) QSort(l,j,a);//继续对左边快排
    if (i<r) QSort(i,r,a);//继续对右边快排
}
int main(){
    int a[10001],n,i;
    cin>>n;
    for(i=1;i<=n;i++) cin>>a[i];
    QSort(1,n,a);
    for(i=1;i<=n;i++) cout<<a[i]<<" ";
    system("pause");
    return 0;
}