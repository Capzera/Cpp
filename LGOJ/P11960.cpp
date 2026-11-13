#include <bits/stdc++.h>
using namespace std;
struct node {
	int x, y;
	bool operator < (const struct node& rhs) const {
		return x - y > rhs.x - rhs.y;
	}
} a[100001];
long long n, ans;
int main() {
	cin >> n;
	for (int i = 1; i <= 2 * n; i++) cin >> a[i].x;
	for (int i = 1; i <= 2 * n; i++) cin >> a[i].y;
	sort(a + 1, a + 2 * n + 1);
	for (int i = 1; i <= n; i++) ans += a[i].x;
	for (int i = n + 1; i <= 2 * n; i++) ans += a[i].y;
	cout << ans;
	return 0;
}
