#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;
int main() {
    int n, m, k;
    long long ans = 0;
    cin >> n >> m >> k;
    vector<int> time(n - 1), timeSt(n + 1), d(n + 1);
    vector<vector<int> > vec(n, vector<int>(3));
    for (int i = 0; i < n - 1; i++) {
        cin >> vec[i][0];
        vec[i][2] = i;
    }
    vector<vector<int> > passengers(m, vector<int>(3));
    for (auto& pass : passengers) {
        for (auto& x : pass) cin >> x;
        vec[pass[1] - 1][1]++;
        vec[pass[2] - 1][1]--;
        timeSt[pass[1]] = max(timeSt[pass[1]], pass[0]);
    }
    sort(passengers.begin(), passengers.end(), [](auto& a, auto& b) {
        return a[1] < b[1] || a[1] == b[1] && a[0] < b[0];
    });
    for (int i = 1; i < n; i++) vec[i][1] += vec[i - 1][1];
    auto cmp = [](vector<int>& a, vector<int>& b) -> bool {
        return a[0] * a[1] < b[0] * b[1];
    };
    priority_queue<vector<int>, vector<vector<int> >, decltype(cmp)> pq(cmp);
    for (int i = 0; i < n - 1; i++) pq.push(vec[i]);
    while (k--) {
        vector<int> p = pq.top(); pq.pop();
        p[0]--;
        pq.push(p);
    }
    while (pq.size()) {
        vector<int> p = pq.top(); pq.pop();
        time[p[2]] = p[0];
    }
    d[1] = timeSt[1];
    for (int i = 2; i <= n; i++) {
        d[i] = max(d[i - 1], timeSt[i - 1]) + time[i - 2];
    }
    for (auto& pass : passengers) {
        ans += d[pass[2]] - pass[0];
    }
    cout << ans << endl;
    system("pause");
    return 0;
}
/*
16 100 1
5 6 5 8 9 6 2 6 9 1 1 9 7 2 4
16 8 14
23 4 14
1 1 13
1 1 8
25 9 16
3 5 12
11 2 4
64 12 14
4 2 15
24 4 12
14 3 13
5 7 10
1 1 2
1 1 2
8 2 3
8 2 13
6 3 4
29 6 7
9 2 12
78 13 16
11 3 5
1 1 2
1 1 2
3 4 5
1 2 5
8 3 11
1 1 2
58 7 13
48 12 13
19 10 14
1 1 2
25 4 6
37 6 9
7 2 10
10 5 6
1 1 7
36 7 8
2 3 6
5 4 12
5 3 9
26 4 5
6 4 14
1 2 10
1 1 7
50 6 9
6 3 4
1 1 3
5 2 12
17 6 8
11 2 14
7 2 7
25 8 10
1 1 2
7 6 10
2 3 4
4 2 15
28 5 9
8 2 10
7 8 16
7 3 5
3 3 14
55 8 16
6 3 12
17 8 9
6 2 4
1 2 3
7 5 6
5 3 11
55 14 15
59 7 8
30 5 9
1 1 10
34 5 7
51 6 12
29 9 10
130 14 15
6 2 3
9 2 7
13 4 6
4 3 12
14 4 12
51 7 8
6 9 15
3 2 14
19 8 13
5 6 12
24 4 6
8 2 9
10 3 4
1 2 7
23 5 8
38 7 16
9 6 13
1 2 3
97 14 15
30 10 14
47 10 12
1 1 9
3 5 10
1 1 14


10 5 0
5 7 2 4 8 2 8 3 1
10 3 4
1 1 3
33 6 9
14 4 8
1 1 4


73
*/