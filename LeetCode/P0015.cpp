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
        if (m[0]==0) m.erase(0);
        map<int,int>::iterator m1=m.begin(),m2=m.end();//m2右指针
        while(m1->first<0)
        {
            map<int,int>::iterator m2=m.end();
            m2--;
            while(m2->first>0)
            {
                int target=0-m1->first-m2->first;
                if (target<m1->first||target>m2->first)
                {
                }
                else if (target==m1->first&&m1->second==2)//目标值为左指针且左指针>1
                {
                    temp[0]=temp[1]=m1->first;temp[2]=m2->first;
                    ans.push_back(temp);
                }
                else if (target==m2->first&&m2->second==2)//目标值为右指针且右指针>1
                {
                    temp[0]=m1->first;temp[1]=temp[2]=m2->first;
                    ans.push_back(temp);
                }
                else if ((target==m1->first&&m1->second<2)||(target==m2->first&&m2->second<2))
                {//目标值不符合
                }
                else if (m.find(target)!=m.end())
                {
                    temp[0]=m1->first;temp[1]=target;temp[2]=m2->first;
                    ans.push_back(temp);
                }
                m2--;
            }
            m1++;
        }
        return ans;
    }
};