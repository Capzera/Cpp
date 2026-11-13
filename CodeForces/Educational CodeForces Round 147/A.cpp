#include <bits/stdc++.h>
using namespace std;
int solve() {
    string str;
    cin >> str;
    if (str[0] == '0') return 0;
    int ans = 1, n = str.size();
    if (str[0] == '?') ans = 9;
    for (int i = 1; i < n; i++) {
        if (str[i] == '?') ans *= 10;
    }
    return ans;
}
int main() {
    int t = 1;
    cin >> t;
    while (t--) {
        cout << solve() << endl;
    }
    return 0;
}