#include <bits/stdc++.h>
#define ull unsigned long long
using namespace std;
const int N = 1e6 + 10, P = 131;
string s;
int n;
ull h[N], p[N];
ull get(int l, int r) {
	return h[r] - h[l - 1] * p[r - l + 1];
}
int check(int len) {
	ull per = get(1, len);
	if (n % len && get(1, n % len) != get(n - n % len + 1, n)) return 0;
	for (int i = 1; (i + 1) * len <= n; i++) {
		if (get(i * len + 1, i * len + len) != per) return 0;
	}
	return 1;
}
int main() {
	cin >> n >> s;
	p[0] = 1;
	for (int i = 1; i <= n; i++) {
		p[i] = p[i - 1] * P;
		h[i] = h[i - 1] * P + s[i - 1];
	}
	for (int len = 1; len <= n; len++) {
		if (check(len)) return cout << len, 0;
	}
	return 0;
}
