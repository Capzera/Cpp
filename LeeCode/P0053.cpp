class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int i;
        vector<int> f(nums.size()+1);
        f[0]=f[nums.size()]=nums[0];
        for(i=1;i<nums.size();i++)
        {
            f[i]=max(nums[i],nums[i]+f[i-1]);
            f[nums.size()]=max(f[nums.size()],f[i]);
        }
        return f[nums.size()];
    }
};