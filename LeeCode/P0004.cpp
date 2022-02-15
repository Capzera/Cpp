class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2)
    {
        int i=0,j=0,now=0,m=nums1.size(),n=nums2.size(),x=0;
        double ans=0;
        int array[2005];
        while (x<(m+n+2)/2)
        {
            x++;
            if (i==m)
            {
                now=nums2[j];
                j++;                
            }
            else if (j==n)
            {
                now=nums1[i];
                i++;                
            }
            else if (nums1[i]<nums2[j])
            {
                now=nums1[i];
                i++;
            }
            else if (nums1[i]>=nums2[j])
            {
                now=nums2[j];
                j++;
            }
            if (x==(m+n+1)/2) ans=now;
        }
        if ((m+n)%2==1) ans=now;
        else ans=(ans+now)/2;
        return ans;
    }
};