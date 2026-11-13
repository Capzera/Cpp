class Solution {
public:
    int rob(vector<int>& nums) {
        int x,y,current,i;
        if (nums.size()==1) return nums[0];
        x=nums[0];y=max(nums[0],nums[1]);
        for(i=2;i<nums.size();i++)
        {
            current=max(x+nums[i],y);
            x=y;
            y=current;
        }
        return max(current,y);
    }
};