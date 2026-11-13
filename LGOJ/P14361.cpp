#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
struct node {
	int a, b, c, mx, sec;
	void read(int &sum, vector<int>& cnt) {
		scanf("%d%d%d", &a, &b, &c);
		if (a >= max(b, c)) {
			mx = 1, sum += a, sec = a - max(b, c);
		} else if (b >= max(a, c)) {
			mx = 2, sum += b, sec = b - max(a, c);
		} else {
			mx = 3, sum += c, sec = c - max(a, b);
		}
		cnt[mx]++;
	}
	bool operator < (const node& rhs) const {
		return sec < rhs.sec;
	}
} a[N];
int t, n;
int solve() {
	scanf("%d", &n);
	int sum = 0, id = -1;
	vector<int> cnt(4);
	for (int i = 1; i <= n; i++) a[i].read(sum, cnt);
	sort(a + 1, a + n + 1);
	for (int id = 1; id <= 3; id++) {
		if (cnt[id] <= n / 2) continue;
		for (int i = 1, j = cnt[id]; i <= n && j > n / 2; i++) {
			if (a[i].mx != id) continue;
			sum -= a[i].sec, j--;
		}
	}
	return sum;
}
int main() {
	cin >> t;
	while (t--) cout << solve() << '\n';
	return 0;
}
