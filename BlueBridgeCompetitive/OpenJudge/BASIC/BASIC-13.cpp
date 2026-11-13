/*
问题描述：
给定一个长度为n的数列，将这个数列按从小到大的顺序排列。1<=n<=200
*/
#include<bits/stdc++.h>
using namespace std;
int main()
{
    vector<int> nums;
    int n,i,put;
    cin>>n;
    for(i=0;i<n;i++) 
    {
        cin>>put;
        nums.push_back(put);
    }
    sort(nums.begin(),nums.end());
    for(i=0;i<n;i++) cout<<nums[i]<<" ";
    cout<<endl;
    system("pause");
    return 0;
}