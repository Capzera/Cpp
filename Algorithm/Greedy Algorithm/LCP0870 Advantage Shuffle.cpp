/*
问题描述：P0870
给定两个大小相等的数组?A?和?B，A 相对于 B 的优势可以用满足?A[i] > B[i]?的索引 i?的数目来描述。
返回?A?的任意排列，使其相对于 B?的优势最大化。
示例 1：
输入：A = [2,7,11,15], B = [1,10,4,11]
输出：[2,11,7,15]
示例 2：
输入：A = [12,24,8,32], B = [13,25,32,11]
输出：[24,32,8,12]
提示：
1 <= A.length = B.length <= 10000
0 <= A[i] <= 10^9
0 <= B[i] <= 10^9
*/
class Solution {
public:
    static bool cmp(const pair<int,int>& a,const pair<int,int>& b) {
        return a.first<b.first;
    }
    vector<int> advantageCount(vector<int>& nums1, vector<int>& nums2) {
        int n=nums1.size(),l=0,r=n-1;
        vector<int> ans(n);
        vector<pair<int,int>> index;
        for (int i=0;i<n;i++) index.push_back({nums2[i],i});//插入{值，索引}键值对
        sort(index.begin(),index.end(),cmp);//对index的值排序
        sort(nums1.begin(),nums1.end());//对nums1排序
        for (int i=0;i<n;i++) {
            if (nums1[i]>index[l].first) {//如果a的最下等可以打赢
                ans[index[l++].second]=nums1[i];//让他们打
            }
            else ans[index[r--].second]=nums1[i];//让a最下等的去打b最强的
        }
        return ans;
    }
};