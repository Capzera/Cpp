/*
问题描述：P2104
给你一个整数数组 nums 。nums 中，子数组的 范围 是子数组中最大元素和最小元素的差值。
返回 nums 中 所有 子数组范围的 和 。
子数组是数组中一个连续 非空 的元素序列。
示例 1：
输入：nums = [1,2,3]
输出：4
解释：nums 的 6 个子数组如下所示：
[1]，范围 = 最大 - 最小 = 1 - 1 = 0 
[2]，范围 = 2 - 2 = 0
[3]，范围 = 3 - 3 = 0
[1,2]，范围 = 2 - 1 = 1
[2,3]，范围 = 3 - 2 = 1
[1,2,3]，范围 = 3 - 1 = 2
所有范围的和是 0 + 0 + 0 + 1 + 1 + 2 = 4
示例 2：
输入：nums = [1,3,3]
输出：4
解释：nums 的 6 个子数组如下所示：
[1]，范围 = 最大 - 最小 = 1 - 1 = 0
[3]，范围 = 3 - 3 = 0
[3]，范围 = 3 - 3 = 0
[1,3]，范围 = 3 - 1 = 2
[3,3]，范围 = 3 - 3 = 0
[1,3,3]，范围 = 3 - 1 = 2
所有范围的和是 0 + 0 + 0 + 2 + 0 + 2 = 4
示例 3：
输入：nums = [4,-2,-3,4,1]
输出：59
解释：nums 中所有子数组范围的和是 59
提示：
1 <= nums.length <= 1000
-109 <= nums[i] <= 109
*/
class Solution {
public:
    long long subArrayRanges(vector<int>& nums) {
        int n = nums.size();
        vector<int> minLeft(n), minRight(n), maxLeft(n), maxRight(n);
        stack<int> minStack, maxStack;
        for (int i = 0; i < n; i++) {
            while (!minStack.empty() && nums[minStack.top()] > nums[i]) {
                minStack.pop();
            }
            minLeft[i] = minStack.empty() ? -1 : minStack.top();
            minStack.push(i);
            
            // 如果 nums[maxStack.top()] == nums[i], 那么根据定义，
            // nums[maxStack.top()] 逻辑上小于 nums[i]，因为 maxStack.top() < i
            while (!maxStack.empty() && nums[maxStack.top()] <= nums[i]) { 
                maxStack.pop();
            }
            maxLeft[i] = maxStack.empty() ? -1 : maxStack.top();
            maxStack.push(i);
        }
        minStack = stack<int>();
        maxStack = stack<int>();
        for (int i = n - 1; i >= 0; i--) {
            // 如果 nums[minStack.top()] == nums[i], 那么根据定义，
            // nums[minStack.top()] 逻辑上大于 nums[i]，因为 minStack.top() > i
            while (!minStack.empty() && nums[minStack.top()] >= nums[i]) { 
                minStack.pop();
            }
            minRight[i] = minStack.empty() ? n : minStack.top();
            minStack.push(i);

            while (!maxStack.empty() && nums[maxStack.top()] < nums[i]) {
                maxStack.pop();
            }
            maxRight[i] = maxStack.empty() ? n : maxStack.top();
            maxStack.push(i);
        }

        long long sumMax = 0, sumMin = 0;
        for (int i = 0; i < n; i++) {
            sumMax += static_cast<long long>(maxRight[i] - i) * (i - maxLeft[i]) * nums[i];
            sumMin += static_cast<long long>(minRight[i] - i) * (i - minLeft[i]) * nums[i];
        }
        return sumMax - sumMin;
    }
};