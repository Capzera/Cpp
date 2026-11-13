#include <bits/stdc++.h>
#define ull unsigned long long
using namespace std;
long long n, m, ans;
ull x;
priority_queue<ull, vector<ull>, greater<ull> > pq;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		pq.push(x);
	}
	while (pq.size() > 1) {
		x = pq.top(); pq.pop();
		ull y = pq.top(); pq.pop();
		pq.push(x + y);
		ans += x >= 1.0 * y / m;
	}
	cout << ans;
	return 0;
}

