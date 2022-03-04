class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int i,j=0,n=nums.size(),zero=0;
        if (n==1) return;
        for(i=0;i<n;i++)
        {
            if (nums[i]!=0) 
            {
                nums[j]=nums[i];
                j++;
            }
            else zero++;
        }
        for(i=n-zero;i<n;i++)nums[i]=0;
    }
};