class Solution {
public:
    bool canJump(vector<int>& nums) {
        int i,j,n=nums.size(),zero=0;
        for(i=n-2;i>=0;i--)
        {
            if (!nums[i]) break;
        }
        if (n==1||i<0) return true;
        j=i-1;
        while (j>=0)
        {
            if (nums[j]>i-j-zero)
            {
                if (nums[j]) zero=1;
                else zero=0;
                i=j;
                j--;
                if (!i) return true;
                continue;
            }
            else j--;
        }
        return false;
    }
};