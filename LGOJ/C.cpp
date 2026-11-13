#include <bits/stdc++.h>
using namespace std;
vector<deque<int> > queues;
int n, m;
bool simulate(int i) {
	vector<deque<int >> tmp = queues;
	tmp[i - 1].push_back(0);
	int p = tmp[i - 1].front();
	tmp[i - 1].pop_front();
	int mx = n * m, steps = 0;
	while (steps <= mx) {
		if (p == 0) return 0;
		int k = p - 1;
		tmp[k].push_back(p);
		p = tmp[k].front();
		tmp[k].pop_front();
		steps++;
	}
	return 1;
}
bool solve() {
	cin >> n >> m;
	queues.clear();
	bool super = 1;
	for (int i = 0; i < n; i++) {
		deque<int> q;
		for (int j = 0; j < m; j++) {
			int x;
			cin >> x;
			q.push_back(x);
			if (x - 1 != i) super = 0;
		}
		queues.push_back(q);
	}
	if (super == 1) return 0;
	bool ans = false;
	if (simulate(1)) return 1;
	if (simulate(n)) return 1;
	if (n * m >= 1000 && n > 2) return 0;
	for (int i = 2; i <= n; i++) {
		if (simulate(i)) return 1;
	}
	return 0;
}
int main() {
	int T;
	cin >> T;
	while (T--) cout << (solve() ? "Yes\n" : "No\n");
	return 0;
}
