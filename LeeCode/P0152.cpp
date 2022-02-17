/*
问题描述：
给你一个整数数组 nums?，请你找出数组中乘积最大的非空连续子数组（该子数组中至少包含一个数字），并返回该子数组所对应的乘积。
测试用例的答案是一个?32-位 整数。
子数组 是数组的连续子序列。
示例 1:
输入: nums = [2,3,-2,4]
输出: 6
解释:?子数组 [2,3] 有最大乘积 6。
示例 2:
输入: nums = [-2,0,-1]
输出: 0
解释:?结果不能为 2, 因为 [-2,-1] 不是子数组。
*/
class Solution {
public:
    int maxsection (vector<int> nums,int l,int r,int minus)
    {
        int sum=1,ans=1;
        if (l==r) return nums[l];
        if (minus%2==0) for(int i=l;i<=r;i++) sum*=nums[i];//负数有偶数个，求区间积
        else//负数有奇数个
        {
            int left=r+1,right=l-1;
            for(int i=l;i<=r;i++)
            {
                if (nums[i]<0)
                {
                    left=min(left,i);//left记录最左边的负数
                    right=max(right,i);//right记录最右边的负数
                }
            }
            for(int i=l;i<right;i++) sum*=nums[i];//去除最左边负数，求积
            for(int i=left+1;i<=r;i++) ans*=nums[i];//同理去除最右边负数
            sum=max(sum,ans);
        }
        return sum;
    }
    int maxProduct(vector<int>& nums) {
        int n=nums.size(),i,minus=0;
        if (n==1) return nums[0];
        map<int,int> zero;//记录0点的位置
        for(i=0;i<n;i++)
        {
            if (nums[i]<0) minus++;//记录零点左边有多少个负数
            if (!nums[i]) 
            {
                zero[i]=minus;
                minus=0;
            }
        }i=0;
        zero[n]=minus;//记录终点左边有多少个负数
        map<int,int>::iterator nextzero=zero.begin();//迭代器记录0点位置
        int current=nums[0];
        while (i<n&&!nums[i])
        {   
            i++;
            nextzero++;//不考虑前导0
        }
        while (i<n)
        {
            current=max(current,maxsection(nums,i,nextzero->first-1,nextzero->second));//计算区间值
            if (nextzero->first==n) break;//计算完最后一组，退出
            else i=nextzero++->first+1;//仍然存在下一个0点，跳到0点后，更新迭代器
        }
        if (zero.size()>1&&current<0) current=0;//有0存在但计算结果小于0，特判
        return current;
    }
};