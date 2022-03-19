#include <bits/stdc++.h>
using namespace std;
int n;
vector<int> nums;
long long  ans=0;
void msort(int l,int r)
{
    if (l>=r) return;
    int mid=l+(r-l)/2;
    msort(l,mid);
    msort(mid+1,r);
    vector<int> tmp;
    int i=l,j=mid+1;
    while (i<=mid&&j<=r) {
        if (nums[i]<=nums[j]) tmp.push_back(nums[i++]);
        else {
            ans+=mid-i+1;
            tmp.push_back(nums[j++]);
        }
    }
    while (i<=mid) tmp.push_back(nums[i++]);
    while (j<=r)  tmp.push_back(nums[j++]);
    for(int i=l,j=0;i<=r;i++) nums[i]=tmp[j++];
}
int main(void)
{
    int put;
    cin>>n;
    nums.resize(n);
    for(int i=0;i<n;i++) cin>>nums[i];
    msort(0,n-1);
    cout<<ans<<endl;
    system("pause");
    return 0;
}