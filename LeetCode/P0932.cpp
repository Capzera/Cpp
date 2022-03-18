/*
问题描述：P0932
对于某些固定的?N，如果数组?A?是整数?1, 2, ..., N?组成的排列，使得：
对于每个?i < j，都不存在?k 满足?i < k < j?使得?A[k] * 2 = A[i] + A[j]。
那么数组 A?是漂亮数组。
给定?N，返回任意漂亮数组?A（保证存在一个）。
示例 1：
输入：4
输出：[2,1,4,3]
示例 2：
输入：5
输出：[3,1,2,5,4]
提示：
1 <= N <= 1000
*/
class Solution {
public:
    unordered_map<int,vector<int>> um;
    vector<int> beautifulArray(int n) {
        if (n==1) return {1};
        if (um.count(n)) return um[n];
        int mid=(n+1)/2;
        vector<int> left=beautifulArray(mid);
        vector<int> right=beautifulArray(n-mid);
        vector<int> ans;
        for (int i:left) ans.push_back(2*i-1);
        for (int i:right) ans.push_back(2*i);
        um[n]=ans;
		return ans;
    }
};