class Solution {
public:
    vector<int> runningSum(vector<int>& nums) {
        int n=nums.size(),i,ans;
        for(i=1;i<n;i++) nums[i]+=nums[i-1];
        return nums;
    }
};