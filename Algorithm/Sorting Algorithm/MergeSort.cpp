/*
    By:Capzera QQ:1786126188 E-Mail:1786126188@qq.com
    归并排序标准算法，本人编写，用于以后题目的算法调用
*/
#include<bits/stdc++.h>
using namespace std;
void Merge(vector<int>& nums,int l,int mid,int r)//合并函数
{
    int n=r-l+1;//待合并区间长度为r-l+1
    int i=0,left=l,right=mid+1;//left指向左半区间，right指向右半区间，i为temp指针
    int* temp=new int[n];//temp临时储存合并好的序列
    while(left<=mid && right<=r)//先将左半或右半取尽
    {
        if (nums[left]<=nums[right]) temp[i++]=nums[left++];
        else temp[i++]=nums[right++];
    }
    while (left<=mid) temp[i++]=nums[left++];//将剩余的数压入temp中
    while (right<=r) temp[i++]=nums[right++];
    for(i=0;i<n;i++) nums[l+i]=temp[i];//将temp的元素拷贝进原nums
    delete [] temp;
}
void MergeSort(vector<int>& nums,int l,int r)
{
    int mid=l+(r-l)/2;
    if (l==r) return;
    MergeSort(nums,l,mid);//划分左半个序列
    MergeSort(nums,mid+1,r);//划分右半个序列
    Merge(nums,l,mid,r);
}
void print(vector<int> nums)
{
    for(int i=0;i<nums.size();i++) cout<<nums[i]<<" ";
    cout<<endl;
}
int main()
{
    int n,i,put;
    cin>>n;
    vector<int> nums(n);
    for(i=0;i<n;i++)
    {
        cin>>put;
        nums[i]=put;
    }
    MergeSort(nums,0,n-1);
    print(nums);
    system("pause");
    return 0;
}