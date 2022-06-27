#include<bits/stdc++.h>

using namespace std;

vector<int> nums;

int n;

void QSort(int l,int r)
{
    if (l>=r) return;
    int i=l,j=r,mid=nums[(l+r)/2];
    while(i<=j) {
        while(nums[i]<mid) i++;
        while(nums[j]>mid) j--;
        if (i<=j)
            swap(nums[i++],nums[j--]);
    }
    QSort(i,r);
    QSort(l,j);
}
int main(){
    cin>>n;
    nums=vector<int> (n);
    for(int i=0;i<n;i++) cin>>nums[i];
    QSort(0,n-1);
    for(int i=0;i<n;i++) cout<<nums[i]<<" ";
    system("pause");
    return 0;
}