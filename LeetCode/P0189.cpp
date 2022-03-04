class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        int i,n=nums.size();
        vector<int> ans;
        k%=n;
        for (i=n-k;i<n;i++) ans.push_back(nums[i]);
        for (i=n-1;i>=k;i--) nums[i]=nums[i-k];
        for (i=0;i<k;i++) nums[i]=ans[i];
    }
};