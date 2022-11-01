/*
问题描述：P0611
给定一个包含非负整数的数组?nums ，返回其中可以组成三角形三条边的三元组个数。
示例 1:
输入: nums = [2,2,3,4]
输出: 3
解释:有效的组合是: 
2,3,4 (使用第一个 2)
2,3,4 (使用第二个 2)
2,2,3
示例 2:
输入: nums = [4,2,3,4]
输出: 4
提示:
1 <= nums.length <= 1000
0 <= nums[i] <= 1000
*/
class Solution {
public:
    int triangleNumber(vector<int>& nums) {
        sort(nums.begin(),nums.end());
        int n=nums.size(),ans=0;
        for (int i=0;i<n-2;i++) {
            for (int j=i+1;j<n-1;j++) {
                int l=j+1,r=n,mid,loc=j;
                while (l<r) {
                    mid=l+(r-l)/2;
                    if (nums[i]+nums[j]>nums[mid]) {
                        l=mid+1;
                        loc=mid;
                    }
                    else r=mid;
                }
                ans+=loc-j;
            }
        }
        return ans;
    }
};