#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
struct node {
	long long x, t, diff;
	void read() {
		cin >> x >> t;
		diff = x > t ? LLONG_MAX : t - x;
	}
	bool operator < (const struct node& rhs) const {
		if (x != rhs.x) return x < rhs.x;
		return t < rhs.t;
	}	
} a[N];
int n;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) a[i].read();
	sort(a + 1, a + n + 1);
	vector<long long> ans;
	for (int i = 1; i <= n; i++) {
		if (a[i].diff == LLONG_MAX) continue;
		if (ans.empty() || a[i].diff >= ans.back()) {
			ans.push_back(a[i].diff);
			continue;
		}
		auto it = upper_bound(ans.begin(), ans.end(), a[i].diff);
		*it = a[i].diff;
	}
	cout << ans.size();
	return 0;
}
