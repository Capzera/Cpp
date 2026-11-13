#include <bits/stdc++.h>
using namespace std;
long long n, m, l, r, lg[50001], f[50001][20];
inline int read() {
	int x = 0, f = 1;
	char ch = getchar();
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		x = (x << 3) + (x << 1) + ch - 48;
		ch = getchar();
	}
	return x * f;
}
struct word {
	string s, p;
	int id;
	bool operator < (const struct word& rhs) const {
		if (p != rhs.p) return p < rhs.p;
		return id > rhs.id;
	}
} a[50001];
int main() {
	n = read(), m = read();
	lg[1] = 0;
	for (int i = 2; i <= n; i++) lg[i] = lg[i >> 1] + 1;
	for (int i = 1; i <= n; i++) {
		cin >> a[i].s;
		a[i].p = a[i].s;
		for (auto& ch : a[i].p) ch = toupper(ch);
		a[i].id = i;
	}
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) f[a[i].id][0] = i;
	for (int j = 1; j <= lg[n]; j++) {
		for (int i = 1; i <= n - (1 << j) + 1; i++) {
			f[i][j] = max(f[i][j - 1], f[i + (1 << j - 1)][j - 1]);
		}
	}
	while (m--) {
		cin >> l >> r;
		int k = lg[r - l + 1];
		cout << a[max(f[l][k], f[r - (1 << k) + 1][k])].s << '\n';
	}
	return 0;
}
