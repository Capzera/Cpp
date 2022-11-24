#include <bits/stdc++.h>
using namespace std;
int main() {
    string s1, s2, ans = "";
    getline(cin, s1);
    getline(cin, s2);
    set<char> s;
    for (auto& c : s2) s.insert(c);
    for (auto& c : s1) {
        if (s.count(c)) continue;
        ans += c;
    }
    cout << ans << endl;
    system("pause");
    return 0;
}