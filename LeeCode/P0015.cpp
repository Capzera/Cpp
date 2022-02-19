/*
问题描述：
给你一个包含 n 个整数的数组?nums，判断?nums?中是否存在三个元素 a，b，c ，使得?a + b + c = 0 ？请你找出所有和为 0 且不重复的三元组。
注意：答案中不可以包含重复的三元组。
示例 1：
输入：nums = [-1,0,1,2,-1,-4]
输出：[[-1,-1,2],[-1,0,1]]
示例 2：
输入：nums = []
输出：[]
示例 3：
输入：nums = [0]
输出：[]
*/
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        map<int,int> m;
        int n=nums.size(),i,j,MAX=INT_MIN;
        vector<vector<int>> ans;
        if (n<3) return ans;//特判，永不符合情况，空传
        vector<int> temp(3);
        for(i=0;i<n;i++) 
        {
            j=nums[i];
            MAX=max(MAX,j);//找最大值
            m[j]++;//记录在哈希表
            if (m[j]>2&&j!=0) m[j]=2;//除了0，每个数最多被记录2次
        }
        if (m[0]>2) ans.push_back(temp);//如果至少有3个0，
        if (m[0]>0) m[0]=1;
        map<int,int>::iterator m1=m.begin(),m2=m.begin();
        while (m2!=m.end()&&m2->first<=0) m2++;
        int left=m1->first,right=m2->first;//选中一对负数和正数，寻找该数对的和并求值
        while (left<0&&right<=MAX)
        {
            while(left<0&&m[left]==0) left++;//left指向当前负数
            while(right<=MAX&&m[right]==0) right++;//right指向当前正数
            
        }
        return ans;
    }
};