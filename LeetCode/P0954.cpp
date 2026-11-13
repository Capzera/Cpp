/*
问题描述：P0954
给定一个长度为偶数的整数数组 arr，只有对 arr 进行重组后可以满足
 “对于每个 0 <=?i < len(arr) / 2，都有 arr[2 * i + 1] = 2 * arr[2 * i]”?时，返回 true；否则，返回 false。
?
示例 1：
输入：arr = [3,1,3,6]
输出：false
示例 2：
输入：arr = [2,1,2,6]
输出：false
示例 3：
输入：arr = [4,-2,2,-4]
输出：true
解释：可以用 [-2,-4] 和 [2,4] 这两组组成 [-2,-4,2,4] 或是 [2,4,-2,-4]
?
提示：
0 <= arr.length <= 3 * 104
arr.length 是偶数
-105 <= arr[i] <= 105
*/
class Solution {
public:
    static bool cmp(int a,int b){
        return abs(a)<abs(b);
    }
    bool canReorderDoubled(vector<int>& arr) {
        unordered_map<int,int> um;
        set<int> s;
        for (int arrs:arr) um[arrs]++;
        if (um.count(0)&&um[0]%2) return false;
        s.insert(arr.begin(),arr.end());
        arr.assign(s.begin(),s.end());
        sort(arr.begin(),arr.end(),cmp);
        for (int i:arr) {
            if (um[i*2]<um[i]) return false;
            else um[i*2]-=um[i];
        }
        return true;
    }
};