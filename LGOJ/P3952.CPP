#include <bits/stdc++.h>
using namespace std;
string solve() {
    int n, target = 0, ans = 0, cnt = 0, flag = -1;
    string origin;
    cin >> n >> origin;
    if (origin != "O(1)") {
        target = stoi(origin.substr(4, origin.size() - 5));//截取次方作为答案
    }
    vector<int> alpha(26), mp(26);
    vector<vector<string> > read(n, vector<string>(4));
    for (auto& r : read) {
        cin >> r[0];
        if (r[0] == "F") cin >> r[1] >> r[2] >> r[3];
    }
    stack<int> stk;
    auto get = [&](string a, string b) -> int {
        int x = a == "n" ? 1100 : stoi(a);//如果a, b为n，则令其为1000，方便做差
        int y = b == "n" ? 1100 : stoi(b);
        return y - x;
    }; 
    for (auto& r : read) {
        if (r[0] == "E") {
            if (stk.empty()) return "ERR";//结束先于For
            auto t = stk.top(); stk.pop();
            alpha[t] = 0;
            if (flag == t) flag = -1;//栈顶为最上层的非法循环,栈顶前的元素都可以提供贡献
            if (mp[t]) {//如果栈顶是复杂度哈希表
                mp[t] = 0;
                cnt--;//层数-1
            }
            continue;
        }
        int k = r[1][0] - 'a', diff = get(r[2], r[3]);
        if (alpha[k]) return "ERR";//循环变量存在过
        alpha[k] = 1, stk.push(k);
        if (flag == -1 && diff < 0) flag = k;//如果无法进入循环，标记当前变量
        if (flag == -1 && diff > 1000) {//复杂度提高1次方
            ans = max(ans, ++cnt);//更新答案
            mp[k] = 1;//加入复杂度哈希表
        }
    }
    if (stk.size()) return "ERR";
    return ans == target ? "Yes" : "No";
}
int main() {
    int t; cin >> t;
    while (t--) cout << solve() << endl;
    system("pause");
    return 0;
}