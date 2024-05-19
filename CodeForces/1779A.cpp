#include <bits/stdc++.h>
using namespace std;
int solve() {
    string s;
    int n;
    cin >> n >> s;
    for (int i = 0; i < n - 1; i++) {
        if (s[i] == 'R' && s[i + 1] == 'L') return 0;
        if (s[i] == 'L' && s[i + 1] == 'R') return i + 1;
    }
    return -1;
}
int main() {
    int t;
    cin >> t;
    while (t--) {
        cout << solve() << endl;
    }
    system("pause");
    return 0;
}