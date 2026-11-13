#include <bits/stdc++.h>
using namespace std;
const int P = 131, m1 = 1e9 + 7, m2 = 1e9 + 9, N = 1e6 + 10;
int n, m, l1, r1, l2, r2;
long long h1[N], h2[N], p1[N], p2[N];
string s;
long long get1(int l, int r) {
	return (h1[r] - h1[l - 1] * p1[r - l + 1] % m1 + m1) % m1;
}
long long get2(int l, int r) {
	return (h2[r] - h2[l - 1] * p2[r - l + 1] % m2 + m2) % m2;
}
int main() {
	cin >> s >> m;
	n = s.size();
	p1[0] = p2[0] = 1;
	for (int i = 1; i <= n; i++) {
		p1[i] = p1[i - 1] * P % m1;
		p2[i] = p2[i - 1] * P % m2;
		h1[i] = (h1[i - 1] * P + s[i - 1]) % m1;
		h2[i] = (h2[i - 1] * P + s[i - 1]) % m2;
	}
	while (m--) {
		scanf("%d%d%d%d", &l1, &r1, &l2, &r2);
		cout << (get1(l1, r1) == get1(l2, r2) && get2(l1, r1) == get2(l2, r2) ? "Yes\n" : "No\n");
	}
	return 0;
}
