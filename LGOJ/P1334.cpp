#include <bits/stdc++.h>
using namespace std;
long long n, x, ans;
priority_queue<long long, vector<long long>, greater<long long>> pq;
int main() {
	cin >> n;
	while (n--) {
		cin >> x;
		pq.push(x);
	}
	while (pq.size() > 1) {
		x = pq.top(); pq.pop();
		long long y = pq.top(); pq.pop();
		pq.push(x + y);
		ans += x + y;
	}
	cout << ans;
	return 0;
}
