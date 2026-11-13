#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
long long n, sa, sb, a[N << 1], b[N], nxt[N];
int main() {
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> a[i];
		sa += a[i];
	}
	for (int i = 0; i < n; i++) {
		cin >> b[i];
		sb += b[i];
	}
	if ((sb - sa) % n) return puts("-1"), 0;
	long long x = (sb - sa) / n;
	for (int i = 0; i < n; i++) {
		a[i] = a[i + n] = a[i] + x;
	}
	int i = 0, j = 0, k = nxt[0] = -1;
	while (j < n) {
		if (k == -1 || b[j] == b[k]) {
			j++, k++;
			nxt[j] = b[j] == b[k] ? nxt[k] : k;
		} else k = nxt[k];
	}
	j = 0;
	while (i < n + n && j < n) {
		if (j == -1 || a[i] == b[j]) {
			i++, j++;
		} else j = nxt[j];
	}
	cout << (j == n ? i - j : -1);
	return 0;
}

