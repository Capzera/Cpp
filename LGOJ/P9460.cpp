#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
int n, k, ans, a[N];
unordered_map<int, int> mp;
priority_queue<int> pq;
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		mp[a[i]]++;
	}
	for (auto& [_, y] : mp) pq.push(y);
	for (int i = 1; i <= k; i++) {
		int t = pq.top(); pq.pop();
		t--;
		pq.push(t);
	}
	if (pq.top() <= k) {
		return puts("pigstd"), 0;
	}
	for (auto& [_, y] : mp) ans += pq.top() <= y + k;
	cout << ans;
	return 0;
}
