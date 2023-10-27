#include <bits/stdc++.h>
using namespace std;
int main() {
    int n = 1000;
    vector<int> vec;
    function<int(int, int, int, int)> dfs = [&](int i, int num, int sum, int target) -> int {
        if (i == vec.size()) {
            if (num + sum == target) return 1;
            return 0;
        }
        if (sum + num * 10 + vec[i] <= target) {
            if (dfs(i + 1, num * 10 + vec[i], sum, target)) return 1;
        }
        if (dfs(i + 1, vec[i], sum + num, target)) return 1;
        return 0;
    };
    cout << "nums = {";
    for (int i = 1; i <= 1000; i++) {
        int sq = i * i;
        vec.clear();
        while (sq) {
            vec.emplace_back(sq % 10);
            sq /= 10;
        }
        reverse(vec.begin(), vec.end());
        if (dfs(0, 0, 0, i)) cout << i << ", ";
    }
    cout << "};";
    system("pause");
    return 0;
}
/*
1 9 10 45 55 99 100 297 703 999 1000
1296
2500
1 2 9 6

29   1
*/