class Solution {
public:
    int largestSumAfterKNegations(vector<int>& nums, int k) {
        sort(nums.begin(),nums.end());
        int i,dmin=0,total=nums[0];
        for(i=1;i<nums.size();i++) total+=nums[i]; 
        for(i=0;i<k;i++)
        {
            total-=2*nums[dmin];
            nums[dmin]-=2*nums[dmin];
            if ((dmin==nums.size()-1)||(nums[dmin+1]>=nums[dmin]))
            {
                k-=i+1;
                if (k%2==0) return total;
                else return total-2*nums[dmin];
            }
            else dmin++;
        }
        return total;
    }
};