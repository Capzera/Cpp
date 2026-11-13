class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
    int fast=1,slow=1,n=nums.size();
    if (!n) return 0;
    for(fast=1;fast<n;fast++)
        if (nums[fast]!=nums[fast-1])
        {
            nums[slow]=nums[fast];
            slow++;
        }
    return slow;
    }
};