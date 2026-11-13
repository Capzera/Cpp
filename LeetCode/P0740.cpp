class Solution {
public:
    int deleteAndEarn(vector<int>& nums) {
        map<int,int> v;
        int n=nums.size(),i,ans=0,MAX=-1,MIN=INT_MAX;
        if (n==1) return nums[0];
        for(i=0;i<n;i++) 
        {
            v[nums[i]]++;
            MAX=max(MAX,nums[i]);//保存要计算的最大值
            MIN=min(MIN,nums[i]);//保存要计算的最小值
        }
        for(i=MIN;i<=MAX;i++) v[i]*=i;//将相同的Key合并计算乘积
        int x=v[MIN],y=v[MIN+1];//X保存<i-2的最大值，Y保存i-1的最大值
        for (i=MIN+2;i<=MAX;i++)
        {
		    ans=max(x+v[i],y);//dp[i]=max(dp[i-2]+v[i],dp[i-1])
            x=max(x,y);
            y=ans;
	    }
        return max(ans,y);//max(dp[max],dp[max-1])可能终点在最后一个，可能终点在倒数第二个
    }
};