#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m, a, b, c;
	cin >> n >> m;
	priority_queue<int> pq;
	while (n--) {
		cin >> a >> b >> c;
		int i = 1, sum = a + b + c;
		for (int i = 1; ; i++) {
			int sum = a * i * i + b * i + c;
			if (pq.size() == m && sum >= pq.top()) break;
			pq.push(sum);
			if (pq.size() > m) pq.pop();
		}
	}
	vector<int> ans;
	while (pq.size()) {
		ans.emplace_back(pq.top());
		pq.pop();
	}
	for (int i = m - 1; i >= 0; i--) cout << ans[i] << " ";
	return 0;
}
