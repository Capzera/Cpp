#include <bits/stdc++.h>
using namespace std;
int main() {
    cin.tie(nullptr)->sync_with_stdio(0);
    vector<string> words, type;
    auto read = [](vector<string>& arr) -> void {
        string s;
        while (getline(cin, s), s != "EOF") {
            string t = "";
            for (auto& ch : s) {
                if (ch != '<') t += ch;
                else if (t.size()) t.pop_back();
            }
            arr.emplace_back(t);
        }
    };
    read(words), read(type);
    int sec, ans = 0, n = min(words.size(), type.size());
    cin >> sec;
    for (int i = 0; i < n; i++) {
        int cnt = 0;
        auto *l = &words[i][0], *r = &type[0];
        while (*l != '\0' && *r !='\0') {
            cnt += *l++ == *r++;
        }
        ans += cnt;
    }
    cout << fixed << setprecision(0) << ans / (second / 60.0) << endl;
    system("pause");
    return 0;
}