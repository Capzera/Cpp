#include <bits/stdc++.h>
using namespace std;
int main() {
    map<string, int> mp;
    int n, flag = 0;
    cin >> n;
    auto get = [] (char x) -> char {
        if (isdigit(x)) return x;
        if (x < 'D') return '2';
        if (x < 'G') return '3';
        if (x < 'J') return '4';
        if (x < 'M') return '5';
        if (x < 'P') return '6';
        if (x < 'T') return '7';
        if (x < 'W') return '8';
        return '9';
    };
    while (n--) {
        string str, ans;
        cin >> str;
        for (auto& ch : str) {
            if (!isdigit(ch) && !isdigit(ch)) continue;
            ans += get(ch);
        }
        mp[ans]++;
    }
    for (auto& [tel, cnt] : mp) {
        if (cnt == 1) continue;
        flag = 1;
        cout << tel.substr(0, 3) << "-" << tel.substr(3) << " " << cnt << " ";
    }
    if (!flag) cout << "No dumplicates." << endl;
    system("pause");
    return 0;
}