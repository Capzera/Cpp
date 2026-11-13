#include <bits/stdc++.h>
using namespace std;
int n, k, ans, a[101];
priority_queue<int, vector<int>, greater<int> > pq;
int main() {
	cin >> k >> n;
	for (int i = 1; i <= k; i++) cin >> a[i];
	pq.push(1);
	while (n--) {
		ans = pq.top(); pq.pop();
		while (pq.size() && ans == pq.top()) pq.pop();
		for (int i = 1; i <= k; i++) {
			long long x = 1ll * a[i] * ans;
			if (x > INT_MAX) break;
			pq.push(x);	
		}
	}
	cout << pq.top();
	return 0;
}
