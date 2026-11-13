#include <bits/stdc++.h>
#define int long long
using namespace std;
const int p = 1e9 + 7;
int n, m, x, k, q, a[601][601];
bool row[601], col[601];
inline long long read() {
	long long ans = 0, f = 1;
	char ch = getchar(); // ch
	while (!isdigit(ch)) {
		if (ch == '-') f = -1;
		ch = getchar();
	}
	while (isdigit(ch)) {
		ans = (ans << 3) + (ans << 1) + ch - '0';
		ch = getchar();
	}
	return f * ans;
}
void solve() {
	k = read();
	memset(row, 0, sizeof(row));
	memset(col, 0, sizeof(col));
	for (int i = 1; i <= k; i++) {
		x = read();
		row[x] = 1;
	}
	for (int i = 1; i <= k; i++) {
		cin >> x;
		col[x] = 1;
	}
	vector<vector<int> > b, c;
	for (int i = 1; i <= n; i++) {
		if (row[i]) continue;
		vector<int> t1;
		for (int j = 1; j <= n; j++) {
			if (col[j]) continue;
			t1.emplace_back(a[i][j]);
		}
		b.emplace_back(t1);
	}
	for (int i = 1; i <= n; i++) {
		if (!row[i]) continue;
		vector<int> t2;
		for (int j = 1; j <= n; j++) {
			if (!col[j]) continue;
			t2.emplace_back(a[i][j]);
		}
		c.emplace_back(t2);
	}
	long long s1 = 1, s2 = 1, s3 = 1, s4 = 1;
	for (int i = 0; i < n - k; i++) {
		s1 = s1 * b[i][i] % p;
		s2 = s2 * b[n - k - i - 1][i] % p;
	}
	int mm = c.size();
	for (int i = 0; i < mm; i++) {
		s3 = s3 * c[i][i] % p;
		s4 = s4 * c[mm - i - 1][i] % p;
	}
	int aa = (s1 - s2 + p) % p;
	int bb = (s3 - s4 + p) % p;
	printf("%lld\n", 1ll * aa * bb % p);
}
signed main() {
	n = read(), q = read();
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			a[i][j] = read();
	while (q--) solve();
	return 0;
}
