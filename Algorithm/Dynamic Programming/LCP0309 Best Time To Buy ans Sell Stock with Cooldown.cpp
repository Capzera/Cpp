/*
问题描述：最佳买卖股票时机含冷冻期
给定一个整数数组prices，其中第prices[i]?表示第?i?天的股票价格 。?
设计一个算法计算出最大利润。在满足以下约束条件下，你可以尽可能地完成更多的交易（多次买卖一支股票）:
卖出股票后，你无法在第二天买入股票 (即冷冻期为 1 天)。
注意：你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。
示例 1:
输入: prices = [1,2,3,0,2]
输出: 3 
解释: 对应的交易状态为: [买入, 卖出, 冷冻期, 买入, 卖出]
示例 2:
输入: prices = [1]
输出: 0
提示：
1 <= prices.length <= 5000
0 <= prices[i] <= 1000
*/
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n=prices.size(),i;
        if (n==1) return 0;//特判，如果只有1个股，利润为0
        vector<vector<int>> dp(n,vector<int>(2));
        dp[0][1]-=prices[0];//在0处持股，买入prices[0]
        dp[1][0]=max(prices[1]-prices[0],0);
        dp[1][1]=max(0-prices[1],0-prices[0]);//在1处持股
        for(i=2;i<n;i++)
        {
            dp[i][0]=max(dp[i-1][1]+prices[i],dp[i-1][0]);//在i处考虑是否卖出,不考虑冷冻期
            dp[i][1]=max(dp[i-2][0]-prices[i],dp[i-1][1]);//加入冷冻期判断
        }
        return dp[n-1][0];
    }
};
//优化空间
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n=prices.size(),negative=0,positive=0-prices[0],prenegative=0,tmp;
        if (n==1) return 0;//特判，如果只有1个股，利润为0
        if (n==2) return max(0,prices[1]-prices[0]);//特判有2个股票的情况
        for(int i=1;i<n;i++)
        {
            tmp=negative;
            negative=max(positive+prices[i],negative);//在i处考虑是否卖出,不考虑冷冻期
            positive=max(prenegative-prices[i],positive);//加入冷冻期判断
            if (i>1)prenegative=tmp;
        }
        return negative;
    }
};