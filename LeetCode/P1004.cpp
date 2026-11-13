/*
问题描述：P1004
给定一个二进制数组?nums?和一个整数?k?，如果可以翻转最多k?个 0 ，则返回 数组中连续 1 的最大个数 。
示例 1：
输入：nums = [1,1,1,0,0,0,1,1,1,1,0], K = 2
输出：6
解释：[1,1,1,0,0,1,1,1,1,1,1]
粗体数字从 0 翻转到 1，最长的子数组长度为 6。
示例 2：
输入：nums = [0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1], K = 3
输出：10
解释：[0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1]
粗体数字从 0 翻转到 1，最长的子数组长度为 10。
提示：
1 <= nums.length <= 105
nums[i]?不是?0?就是?1
0 <= k <= nums.length
*/
class Solution {
public:
    int longestOnes(vector<int>& nums, int k) {
        int n=nums.size(),ans=k? 1:0;
        vector<int> sum(n+1);
        for (int i=1;i<=n;i++) sum[i]=sum[i-1]+nums[i-1];
        int l=0,r=n+1,mid=ans;
        while (l<r) {
            mid=l+(r-l)/2;//mid保存二分答案
            int step=0;//step记录当前答案是否有解
            for (int i=0;i<n-mid+1;i++) {
                if (mid-(sum[i+mid]-sum[i])<=k) {//判断需要操作的次数是否小于等于k
                    step=1;//可行
                    ans=max(ans,mid);//记录答案
                    break;//剪枝
                }
            }
            if (step) l=mid+1;//答案可行，向右二分
            else r=mid;//答案非法，向左二分
        }
        return ans;
    }
};