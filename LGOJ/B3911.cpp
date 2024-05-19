#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m;
    string s, t;
    cin >> n >> m;
    unordered_map<string, string> mp;
    while (n--) {
        cin >> s >> t;
        mp[s] = t;
    }
    cin.ignore();
    while (m--) {
        getline(cin, s);
        t = "";
        for (int i = 0, flag = 0; i < s.size(); i++) {
            if (s[i] == '}') {
                flag = 0;
                cout << mp[t];
                continue;
            }
            if (s[i] == '{') {
                flag = 1;
                t = "";
                continue;
            }
            if (!flag) cout << s[i];
            else t += s[i];
        }
        cout << endl;
    }
    return 0;
}