#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, q, l, r;
    cin >> n >> q;
    set<int> ret;
    for (int i = 0; i < n; i++) {
        string s, tmp = "";
        int ex = 0, nx = 0, op = 1;
        cin >> s;
        for (auto& ch : s) {
            if (ch == 'x') {
                ex = stoi(tmp) * op;
                tmp = "";
            }
            else if (ch == '=') {
                nx = stoi(tmp) * op * -1;
                op = 1;
                tmp = "";
            }
            else if (ch == '-') op = -1;
            else if (ch == '+') op = 1;
            else tmp += ch;
        }
        nx += stoi(tmp) * op;
        ret.emplace(nx / ex);
    }
    vector<int> num(ret.begin(), ret.end());
    while (q--) {
        cin >> l >> r;
        int x = lower_bound(num.begin(), num.end(), l) - num.begin();
        int y = lower_bound(num.begin(), num.end(), r + 1) - num.begin();
        cout << y - x << endl;
    }
    system("pause");
    return 0;
}