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
        int n = nums.size();
        sort(nums.begin(),nums.end());
        vector<vector<int>> ans;
        for (int first = 0; first < n; ++first)//枚举a
        {
            // 需要和上一次枚举的数不相同
            if (first > 0 && nums[first] == nums[first - 1]) continue;// c 对应的指针初始指向数组的最右端
            int third = n - 1;
            int target = -nums[first];
            for (int second = first + 1; second < n; ++second)//枚举b
            {
                // 需要和上一次枚举的数不相同
                if (second > first + 1 && nums[second] == nums[second - 1]) continue;// 需要保证 b 的指针在 c 的指针的左侧
                while (second < third && nums[second] + nums[third] > target) --third;
                // 如果指针重合，随着 b 后续的增加就不会有满足 a+b+c=0 并且 b<c 的 c 了，可以退出循环
                if (second == third) break;
                if (nums[second] + nums[third] == target) ans.push_back({nums[first], nums[second], nums[third]});
            }
        }
        return ans;
    }
};
