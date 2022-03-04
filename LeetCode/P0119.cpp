/*
问题描述：P0119
给定一个非负索引 rowIndex，返回「杨辉三角」的第 rowIndex?行。
在「杨辉三角」中，每个数是它左上方和右上方的数的和。
示例 1:
输入: rowIndex = 3
输出: [1,3,3,1]
示例 2:
输入: rowIndex = 0
输出: [1]
示例 3:
输入: rowIndex = 1
输出: [1,1]
提示:
0 <= rowIndex <= 33
*/
//动态规划，时间复杂度O(rowIndex^2)，空间复杂度O(1)
class Solution {
public:
    vector<int> getRow(int rowIndex) {
        vector<int> dp(rowIndex+1);
        dp[0]=1;
        for(int i=1;i<=rowIndex;i++)
        {
            dp[i]=1;
            for(int j=i-1;j>0;j--) dp[j]+=dp[j-1];
        }
        return dp;
    }
};
//数学方法，时间复杂度O(rowIndex)，空间复杂度O(1)
class Solution {
public:
    vector<int> getRow(int rowIndex) {
        vector<int> dp(rowIndex+1);
        dp[0]=1;
        if (!rowIndex) return dp;
        dp[rowIndex]=1;
        for(int i=1;i<=rowIndex/2;i++) dp[i]=dp[rowIndex-i]=1LL*dp[i-1]*(rowIndex-i+1)/i;
        return dp;
    }
};