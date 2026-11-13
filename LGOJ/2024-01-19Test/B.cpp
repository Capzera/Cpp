#include <bits/stdc++.h>
using namespace std;
priority_queue<long long, vector<long long>, greater<long long> > pq;
int n, m, ans = 0;
long long x;
int main() {
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        cin >> x;
        if (pq.size() < m) {
            pq.push(x);
        }
        else if (x > pq.top()) {
            pq.pop();
            pq.push(x);
            ans++;
        }
    }
    cout << ans << endl;
    while (pq.size()) {
        cout << pq.top() << " ";
        pq.pop();
    }
    system("pause");
    return 0;
}