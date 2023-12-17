#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, T;
    cin >> n >> T;
    vector<int> a(n + 1);
    priority_queue<long long, vector<long long>, greater<long long> > pq;
    long long b, ans = 0, sum = 0;
    for (int i = 1; i <= n; i++) cin >> a[i];
    for (int i = 1; i <= n; i++) {
        cin >> b;
        T -= 2;
        while (pq.size() > T) {
            sum -= pq.top(); pq.pop();
        }
        while (pq.size() < T || pq.top() < a[i]) {
            if (pq.size() == T) {
                sum -= pq.top(); pq.pop();
            }
            pq.push(a[i]);
            sum += a[i];
            a[i] -= b;
            if (a[i] <= 0) break;
        }
        ans = max(ans, sum);
        cout << ans << " ";
    }
    cout << endl;
    system("pause");
    return 0;
}