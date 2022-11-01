class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        vector<int> sum;
        int i;
        double ans=-DBL_MAX;
        sum.resize(nums.size()+1);
        for(i=0;i<nums.size();i++)
        {
            sum[i+1]=sum[i]+nums[i];
            if (i>=(k-1)) ans=fmax(ans,(double)(sum[i+1]-sum[i-k+1])/k);
        }
        return ans;
    }
};