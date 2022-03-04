class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        int dp[2505],n=nums.size(),i,j,ans,ret=0;
        if (n==1) return 1;
        dp[0]=1;
        for(i=1;i<n;i++)
        {
            ans=0;
            for(j=0;j<i;j++)
            {
                if (nums[j]<nums[i])
                    ans=max(ans,dp[j]);
            }
            dp[i]=ans+1;
            ret=max(ret,dp[i]);
        }
        return ret;
    }
};