class Solution {
public:
    vector<int>ans,preSum;
    int PS(int x,int k)
{
    return preSum[x+1]-preSum[x-k+1];
}
    vector<int> maxSumOfThreeSubarrays(vector<int>& nums, int k) {
        int i=3*k-1,j,s1,s2,n=nums.size(),tot2=0,tot1=0,maxtot=-1;
        ans.resize(3);
        ans={0,0,0};
        preSum.resize(n+1);
        for(j=1;j<=n;j++) preSum[j]=preSum[j-1]+nums[j-1];
        for( ;i<n;i++)
        {
            if (PS(i-2*k,k)>tot1)
            {
                s1=i-2*k;
                tot1=PS(s1,k);
            }
            if (PS(i-k,k)+tot1>tot2)
            {
                j=i-k;
                s2=s1;
                tot2=PS(j,k)+tot1;
            }
            if (PS(i,k)+tot2>maxtot)
            {
                maxtot=PS(i,k)+tot2;
                ans={s2-k+1,j-k+1,i-k+1};
            }
        }
        return ans;
    }
};