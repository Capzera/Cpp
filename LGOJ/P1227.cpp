#include <bits/stdc++.h>
using namespace std;
const int N = 2e4 + 5;
struct node {
	int x, y;
	bool operator < (const struct node& rhs) const {
		if (y != rhs.y) return y < rhs.y;
		return x < rhs.x;
	}
} a[N];
int n;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i].x >> a[i].y;
	}
	sort(a + 1, a + n + 1);
	int tx = a[1].x + a[n].x;
	int ty = a[1].y + a[n].y;
	for (int i = 2, j = n - 1; i <= j; i++, j--) {
		if (a[i].x + a[j].x != tx || a[i].y + a[j].y != ty) return puts("This is a dangerous situation!"), 0;
	}
	printf("V.I.P. should stay at (%.1lf,%.1lf).", tx / 2.0, ty / 2.0);
	return 0;
}
