#include<bits/stdc++.h>
using namespace std;
int main() {
    string s, t;
    cin >> s >> t;
    int m = s.size(), n = t.size(), i = 0, j = 0, ans = 0;
    while (i < m && j < n) {
        while (i < m && j < n && s[i] == t[j]) {
            i++, j++, ans++;
        }
        i++;
    }
    cout << ans << endl;
    system("pause");
    return 0;
}