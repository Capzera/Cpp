#include <bits/stdc++.h>
using namespace std;
void solve() {
	int n, b, x;
	cin >> n;
	vector<int> c(n);
	priority_queue<vector<int>> pq, pq2;
	for (auto& x : c) cin >> x;
	for (int i = 0; i < n; i++) {
		cin >> x;
		if (c[i] == 1) pq.push({x, 1});
		if (c[i] == 2) pq.push({x, 2});
		if (c[i] == 3) pq2.push({x, 3});
	}
	double suma = 0, sumb = 0;
	for (int i = 0; i < 8; i++) {
		cin >> b;
		sumb += b;
	}
	if (pq2.empty()) {
		cout << "No" << endl;
		return;
	}
	suma = pq2.top()[0]; pq2.pop();
	int c1 = 0, c2 = 1;
	for (int cnt = 1; cnt < 8; cnt++) {
		if (pq.empty() && pq2.empty()) {
			suma = -1;
			break;
		}
		int x = -1, y = -1, cx = -1, cy = -1;
		if (pq.size()) x = pq.top()[0], cx = pq.top()[1];
		if (pq2.size()) y = pq2.top()[0], cy = pq2.top()[1];
		if (x >= y) {
			pq.pop();          
			if (c1 == 1 && cx == 2) {
				cnt--;
				continue;
			}
			if (cx == 2) c1++;
			suma += x;
		} else {
			pq2.pop();
			if (c2 == 3) {
				cnt--;
				continue;
			}
			c2++;
			suma += y;
		}
	}
	cout << (suma + 16 >= sumb ? "Yes" : "No") << endl;
}
int main() {
	int t;
	cin >> t;
	while (t--) {
		solve();
	}
	return 0;
}
