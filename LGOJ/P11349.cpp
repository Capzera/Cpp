#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 10;
struct node {
	int x, y;
	void read() {
		cin >> x >> y;
	}
	bool operator < (const struct node& rhs) const {
		return x > rhs.x;
	}
}a[N], b[N];
int n, m, mx[N];
long long ans;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) a[i].read();
	for (int i = 1; i <= m; i++) b[i].read();
	sort(a + 1, a + n + 1), sort(b + 1, b + m + 1);
	for (int i = n; i >= 1; i--) mx[i] = max(mx[i + 1], a[i].y);
	for (int i = 1, j = 1; i <= m; i++) {
		while (j <= n && a[j].x > b[i].x) j++;
		if (j > n) break;
		ans += max(mx[j] - b[i].y, 0);
	}
	cout << ans;
	return 0;
}
/*

8 3
2 5
1 1


9 4
3 2
1 3
*/
