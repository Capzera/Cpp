/*
问题描述：P0564
给定一个表示整数的字符串?n ，返回与它最近的回文整数（不包括自身）。如果不止一个，返回较小的那个。
“最近的”定义为两个整数差的绝对值最小。
示例 1:
输入: n = "123"
输出: "121"
示例 2:
输入: n = "1"
输出: "0"
解释: 0 和 2是最近的回文，但我们返回最小的，也就是 0。
提示:
1 <= n.length <= 18
n?只由数字组成
n?不含前导 0
n?代表在?[1, 1018?- 1] 范围内的整数
*/
class Solution {
public:
    vector<long> getCandidates(const string& n) {
        int len = n.length();
        vector<long> candidates = {
            (long)pow(10, len - 1) - 1,
            (long)pow(10, len) + 1,
        };
        long selfPrefix = stol(n.substr(0, (len + 1) / 2));
        for (int i : {selfPrefix - 1, selfPrefix, selfPrefix + 1}) {
            string prefix = to_string(i);
            string candidate = prefix + string(prefix.rbegin() + (len & 1), prefix.rend());
            candidates.push_back(stol(candidate));
        }
        return candidates;
    }

    string nearestPalindromic(string n) {
        long selfNumber = stol(n), ans = -1;
        const vector<long>& candidates = getCandidates(n);
        for (auto& candidate : candidates) {
            if (candidate != selfNumber) {
                if (ans == -1 ||
                    abs(candidate - selfNumber) < abs(ans - selfNumber) ||
                    abs(candidate - selfNumber) == abs(ans - selfNumber) && candidate < ans) {
                    ans = candidate;
                }
            }
        }
        return to_string(ans);
    }
};