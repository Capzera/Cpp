#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m, w, ans = 0;
    cin >> n >> m;
    priority_queue<int, vector<int>, greater<int> > pq;
    for (int i = 0; i < m; i++) pq.push(0);
    while (n--) {
        cin >> w;
        auto mn = pq.top(); pq.pop();
        pq.push(mn + w);
    }
    while (pq.size()) {
        ans = max(ans, pq.top());
        pq.pop();
    }
    cout << ans << endl;
    system("pause");
    return  0;
}