#include <bits/stdc++.h>
using namespace std;
int n, m;
vector<int> prime;
int st[100010], cnt_p[100010];
vector<int> mul(vector<int> &a, vector<int> b) {
	vector<int> res(100);
	for (int i = 99; i >= 0; i--) {
		if (!a[i]) continue;
		for (int j = 99; j >= 99 - i; j--) {
			long long x = a[i] * b[j];
			int p = i + j - 99;
			while (x && p >= 0) {
				x += res[p];
				res[p] = x % 10;
				x /= 10;
				p--;
			}
		}
	}
	return res;
}
vector<int> pow(int a, int b) {
	vector<int> res(100), base(100); res.back()++;
	string s = to_string(a);
	for (int i = 0; i < s.size(); i++) {
		base[100 - s.size() + i] = s[i] - '0';
	}
	while (b) {
		if (b & 1) res = mul(res, base);
		base = mul(base, base);
		b >>= 1;
	}
	return res;
}
void calc(int n, int k) {
	for (int p : prime) {
		if (p > n) break;
		long long x = n, cnt = 0;
		while (x >= p) {
			cnt += x / p;
			x /= p;
		}
		cnt_p[p] += cnt * k;
	}
}
int main() {
	cin >> n >> m;
	for (int i = 2; i <= n + m; i++) {
		if (!st[i]) prime.push_back(i);
		for (int j = 0; j < prime.size() && 1ll * i * prime[j] <= n + m; j++) {
			st[i * prime[j]] = 1;
			if (i % prime[j] == 0) break;
		}
	}
	calc(n + m, 1), calc(n, -1), calc(m, -1);
	vector<int> ans(100); ans.back()++;
	for (int p : prime) {
		if (cnt_p[p]) {
			ans = mul(ans, pow(p, cnt_p[p]));
		}
	}
	for (int i = 1; i <= 100; i++) {
		cout << ans[i - 1];
		if (i % 10 == 0) putchar('\n');
	}
	return 0;
}
