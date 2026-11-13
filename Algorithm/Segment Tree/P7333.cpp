#include <bits/stdc++.h>
using namespace std;
const int N = 3e5 + 10;
int n, m, x, f[N][20], lg[N];
inline int read() {
	int x = 0;
	char ch = getchar();
	while (!isdigit(ch)) ch = getchar();
	while (isdigit(ch)) {
		x = (x << 1) + (x << 3) + (ch ^ 48);
		ch = getchar();
	}
	return x;
}
inline int query(int l, int r) {
	int k = lg[r - l + 1];
	return max(f[l][k], f[r - (1 << k) + 1][k]);
}
int check(int x, int i) {
	int left = query(i - x, i - 1);
	int right = query(i + 1, i + x);
	return max(left, right);
}
int main() {
	n = read();
	lg[0] = -1;
	for (int i = 1; i <= n; i++) {
		f[i][0] = f[i + n][0] = f[i + n + n][0] = read();
	}
	for (int i = 1; i <= 3 * n; i++) lg[i] = lg[i >> 1] + 1;
	for (int j = 1; j <= lg[n << 1]; j++) {
		for (int i = 1; i <= 3 * n - (1 << j) + 1; i++) {
			f[i][j] = max(f[i][j - 1], f[i + (1 << j - 1)][j - 1]);
		}
	}
	for (int i = 1; i <= n; i++) {
		x = read();
		int l = 1, r = n, ans = -1;
		while (l < r) {
			int mid = l + (r - l >> 1);
			if (check(mid, i + n) >= x) {
				ans = r = mid;
			} else l = mid + 1;
		}
		cout << ans << " ";
	}
	return 0;
}
