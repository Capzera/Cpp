class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n=prices.size(),dp=prices[0],Max=0,i;
        if (n==1) return 0;
        for(i=1;i<n;i++)
        {
            dp=min(dp,prices[i]);
            Max=max(prices[i]-dp,Max);
        }
        return Max;
    }
};