class Solution {
public:
    int rob(vector<int>& nums) {
        int n=nums.size(),i,f[102],ans;
        if (n==1) return nums[0];
        if (n==2) return max(nums[0],nums[1]);
        f[0]=nums[0];f[1]=max(nums[0],nums[1]);
        for(i=2;i<n-1;i++) f[i]=max(f[i-2]+nums[i],f[i-1]);
        ans=f[n-2];
        f[1]=nums[1];f[2]=max(nums[1],nums[2]);
        for(i=3;i<n;i++) f[i]=max(f[i-2]+nums[i],f[i-1]);
        return max(ans,f[n-1]);
    }
};