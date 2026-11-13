#include <bits/stdc++.h>
using namespace std;
int get (string& a, string& b) {
    int x = a == "n" ? 1100 : stoi(a);
    int y = b == "n" ? 1100 : stoi(b);
    return y - x;
}
string solve() {
    int n, target = 0, ans = 0, cnt = 0, flag = -1;
    stack<int> stk;
    vector<int> alpha(26), devote(26);
    string origin;
    cin >> n >> origin;
    if (origin != "O(1)") {
        target = stoi(origin.substr(4));
    }
    vector<vector<string> > read(n, vector<string>(4));
    for (auto& r : read) {
        cin >> r[0];
        if (r[0] == "F") cin >> r[1] >> r[2] >> r[3];
    }
    for (auto& r : read) {
        if (r[0] == "E") {
            if (stk.empty()) return "ERR";
            auto t = stk.top(); stk.pop();
            alpha[t] = 0;
            if (flag == t) flag = -1;
            if (devote[t]) {
                devote[t] = 0;
                cnt--;
            }
            continue;
        }
        int k = r[1][0] - 'a', diff = get(r[2], r[3]);
        if (alpha[k]) return "ERR";
        alpha[k] = 1, stk.push(k);
        if (flag == -1 && diff < 0) flag = k;
        if (flag == -1 && diff > 1000) {
            ans = max(ans, ++cnt);
            devote[k] = 1;
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