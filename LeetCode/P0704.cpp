class Solution {
public:
    int search(vector<int>& nums, int target) {
        int l=0,r=nums.size(),mid=(l+r)/2;
        while (l<mid)
        {
            if (nums[mid]<target) l=mid+1;
            if (nums[mid]>target) r=mid;
            if (nums[mid]==target) return mid;
            mid=(l+r)/2;
        }
        if (nums[mid]==target)return mid;
        else return -1;
    }
};