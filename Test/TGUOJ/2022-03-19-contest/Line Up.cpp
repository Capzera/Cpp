/*
问题描述：
幼儿园老师要带n个小朋友去郊游。这n个小朋友随机地站成了一排。为了让队伍看起来比较整齐，老师想让大家按照从小到大的顺序排队。
现在给出了n个小朋友的身高，请你帮老师找一找不满足从小到大排好序的有多少对。（即寻找i < j 且 ai > aj的对数）
输入
输入数据包括两行
第一行包含一个正整数n, 表示人数 (1 <= n <= 100000)
第二行包含n个正整数a1, a2, ..., an, 表示从队首到队尾每个小朋友的身高(1 <= ai <= 10^9)
输出
输出一个整数, 表示不满足题目要求的对数
输入样例 1 
6
3 2 4 5 6 1
输出样例 1
6
*/
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
int main()
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