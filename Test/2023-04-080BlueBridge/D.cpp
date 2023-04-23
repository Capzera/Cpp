#include <bits/stdc++.h>
#define YN(ans) printf("%s\n", (ans) ? "YES" : "NO")
using namespace std;
bool cmp(vector<int>& a, vector<int>& b) {
    if (a[0] + a[1] == b[0] + b[1]) return b[0] < b[1];
    return a[0] + a[1] < b[0] + b[1];
}
bool solve() {
    int n;
    cin >> n;
    vector<vector<int>> times(n, vector<int> (3));
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < 3; j++) cin >> times[i][j];
    sort(times.begin(), times.end(), cmp);
    int curtime = 0;
    for (auto& time : times) {
        if (curtime > time[0] + time[1]) return 0;
        curtime = max(curtime, time[0]) + time[2];
    }
    return 1;
}

int main() {
    int t = 1;
    cin >> t;
    while (t--) {
        YN(solve());
    }
    system("pause");
    return 0;
}