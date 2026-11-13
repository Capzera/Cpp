#include <bits/stdc++.h>
using namespace std;
int main() {
    string s;
    int ans = 0;
    cin >> s;
    for (auto& x : s) x = toupper(x);
    for (int i = 1; i < s.size(); i++) {
        ans += s[i] != s[i - 1];
    }
    cout << ans << endl;
    system("pause");
    return 0;
}