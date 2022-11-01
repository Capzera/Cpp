class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int fast=0,slow=0,n=nums.size();
        if (!n) return 0;
        for (fast=0;fast<n;fast++)
        {
            if (nums[fast]!=val)
            {
                nums[slow]=nums[fast];
                slow++;
            }
        }
        return slow;
    }
};