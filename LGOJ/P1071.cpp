#include <bits/stdc++.h>
using namespace std;

string solve() {
    string a, b, c;
    cin >> a >> b >> c;
    unordered_map<char, char> mp;
    unordered_set<char> alpha;
    for (int i = 0; i < a.size(); i++) {
        alpha.emplace(b[i]);
        if (mp.count(a[i]) && mp[a[i]] != b[i]) return "Failed";
        mp[a[i]] = b[i];
    }
    if (alpha.size() != 26) return "Failed";
    for (auto& ch : c) {
        ch = mp[ch];
    }
    return c;
}

int main() {
    cout << solve() << endl;
    system("pause");
    return 0;
}