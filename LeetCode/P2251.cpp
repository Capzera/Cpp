/*
问题描述：P2251
给你一个下标从 0?开始的二维整数数组?flowers?，
    其中?flowers[i] = [starti, endi]?表示第?i?朵花的 花期?从?starti?到?endi?（都 包含）。
同时给你一个下标从 0?开始大小为 n?的整数数组?persons?，persons[i]?是第?i?个人来看花的时间。
请你返回一个大小为 n?的整数数组?answer?，其中?answer[i]是第?i?个人到达时在花期内花的?数目?。
示例 1：
输入：flowers = [[1,6],[3,7],[9,12],[4,13]], persons = [2,3,7,11]
输出：[1,2,2,2]
解释：上图展示了每朵花的花期时间，和每个人的到达时间。
对每个人，我们返回他们到达时在花期内花的数目。
示例 2：
输入：flowers = [[1,10],[3,3]], persons = [3,3,2]
输出：[2,2,1]
解释：上图展示了每朵花的花期时间，和每个人的到达时间。
对每个人，我们返回他们到达时在花期内花的数目。
提示：
1 <= flowers.length <= 5 * 104
flowers[i].length == 2
1 <= starti <= endi <= 109
1 <= persons.length <= 5 * 104
1 <= persons[i] <= 109
*/
class Solution {
    typedef pair<int, int> pii;
    const int INF = 1e9;
public:
    vector<int> fullBloomFlowers(vector<vector<int>>& flowers, vector<int>& persons) {
        vector<pii> vec;
        for (auto &f : flowers) vec.push_back({f[0], -INF}), vec.push_back({f[1], INF});
        for (int i = 0; i < persons.size(); i++) vec.push_back({persons[i], i});
        sort(vec.begin(), vec.end());
        vector<int> ans(persons.size());
        int now = 0;
        for (pii p : vec) {
            if (p.second<0) now++;
            else if (p.second == INF) now--;
            else ans[p.second] = now;
        }
        return ans;
    }
};