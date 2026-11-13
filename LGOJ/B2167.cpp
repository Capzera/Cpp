#include <bits/stdc++.h>
using namespace std;
int read() {
	int ans = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		ans = ans * 10 + ch - '0';
		ch = getchar();
	}
	return ans;
}
int n, q, x, a[1000010];
int main() {
	n = read(), q = read();
	for (int i = 1; i <= n; i++) a[i] = read();
	while (q--) {
		x = read();
		int l = 1, r = n + 1, ans = -1;
		while (l < r) {
			int mid = l + (r - l >> 1);
			if (a[mid] <= x) {
				if (a[mid] == x) ans = mid;
				l = mid + 1;
			} else r = mid;
		}
		printf("%d\n", ans);
	}
	return 0;
}
