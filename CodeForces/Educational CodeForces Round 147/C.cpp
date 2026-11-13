#pragma GCC optimize(2)
#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f
void solve() {
    string s;
    cin >> s;
    int maxtime = 0, ans = inf, n = s.size();
    for (int c = 'a'; c <= 'z'; c++) {
        int cnt = 0, maxcnt = 0, score = 0;
        for (auto& ch : s) {
            if (ch != c) cnt++;
            else {
                maxcnt = cnt > maxcnt ? cnt : maxcnt;
                cnt = 0;
            }
        }
        maxcnt = max(maxcnt, cnt);
        if (maxcnt) score = log(maxcnt) / log(2) + 1;
        ans = score < ans ? score : ans;
    }
    cout << ans << endl;
}

int main(){
    int t = 1;
    cin >> t;
    while (t--){
        solve();
    }
    system("pause");
    return 0;
}