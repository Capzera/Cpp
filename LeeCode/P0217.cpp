class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        unordered_set<int> count;
        count.insert(nums.begin(),nums.end());
        return nums.size()>count.size();
    }
};