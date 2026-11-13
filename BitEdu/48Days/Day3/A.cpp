class Solution {
public:
    int MoreThanHalfNum_Solution(vector<int>& nums) {
        int cnt = 0, ans = 0;
        for (auto& i : nums) {
            if (!cnt) ans = i;
            if (i == ans) cnt++;
            else cnt--;
        }
        return ans;
    }
};
/*
input = {
    [1,2,3,2,2,2,5,4,2]
    [3,3,3,3,2,2,2]
    [1]
}
*/