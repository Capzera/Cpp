class Solution {
public:
    int singleNonDuplicate(vector<int>& nums) {
        int l=0,r=nums.size(),mid;
        if (r==1) return nums[0];
        if (nums[0]!=nums[1]) return nums[0];
        while (l<r)
        {
            mid=l+(r-l)/2;
            if (nums[mid]==nums[mid-1])
            {
                if (mid%2==0) r=mid;
                else l=mid+1;
            }
            else
            {
                if (nums[mid]==nums[mid+1]) 
                {
                    if (nums[mid-1]!=nums[mid-2]) return nums[mid-1];
                    else
                    {
                        if (mid%2==0) l=mid+1;
                        else r=mid;
                    }
                }
                else return nums[mid];
            }
        }
        return nums[r-1];
    }
};