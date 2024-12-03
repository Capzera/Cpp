#include <bits/stdc++.h>
using namespace std;
long long n, x, a[1001], cnt, win, draw, lose, ans;
int main() {
	cin >> n;
	priority_queue<int> pq;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) {
		cin >> x;
		if (a[i] > x) win++;
		if (a[i] == x) pq.push(0);
		if (a[i] < x) lose++, pq.push(a[i] - x);
	}
	while (win <= lose) {
		auto cur = pq.top(); pq.pop();
		if (!cur) {
			ans++, win++;
		} else {
			ans -= cur, lose--, pq.push(0);
		}
	}
	cout << ans;
	return 0;
}
