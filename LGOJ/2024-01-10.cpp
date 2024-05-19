#include <bits/stdc++.h>
using namespace std;
string solve(long long k, long long target) {
    if (1ll * k * target <= 0) return "false";
    if (k < 0) {
        k = -k, target = -target;
    }
    priority_queue<long long, vector<long long>, greater<long long> > pq;
    pq.push(k);
    while (pq.top() < target) {
        long long t = pq.top(); pq.pop();
        long long x = 2ll * t + 1, y = 3ll * t + 1;
        if (x == target) return "true";
        if (y == target) return "true";
        pq.push(x), pq.push(y);
    }
    return "false";
}
int main() {
    long long k, target;
    cin >> k >> target;
    cout << solve(k, target) << endl;
    system("pause");
    return 0;
}