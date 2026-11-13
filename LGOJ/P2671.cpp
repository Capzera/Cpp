#include <bits/stdc++.h>
using namespace std;
const int p = 1e4 + 7;
long long n, m, c, ans, num[100001];
vector<pair<long long, long long> > odd[100001], even[100001];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) cin >> num[i];
	for (int i = 1; i <= n; i++) {
		cin >> c;
		if (i & 1) odd[c].push_back({num[i], i});
		else even[c].push_back({num[i], i});
	}
	for (int c = 1; c <= m; c++) {
		long long sn = odd[c][0].first, si = odd[c][0].second, sp = sn * si % p;
		for(int i = 1; i < odd[c].size(); i++) {
			long long x = odd[c][i].first, idx = odd[c][i].second;
			sn = (sn + x) % p, si = (si + idx) % p;
			ans = (ans + x * si % p + idx * sn % p + sp) % p;
			sp = (sp + x * idx % p) % p;
		}
		sn = even[c][0].first, si = even[c][0].second, sp = sn * si % p;
		for(int i = 1; i < even[c].size(); i++) {
			long long x = even[c][i].first, idx = even[c][i].second;
			sn = (sn + x) % p, si = (si + idx) % p;
			ans = (ans + x * si + idx * sn + sp) % p;
			sp = (sp + x * idx % p) % p;
		}
	}
	cout << ans;
	return 0;
}
n2(f1 + f2) + f2(n1 + n2) + f1n1
f1n2 + f2n2 + f2n1 + f2n2 + f1n1
(f1 + f2)(n1 + n2)
f1n1 + f1n2 + f2n1 + f2n2

n1  f1   f1n1
n2(f1 + f2) + f2(n1 + n2) + f1n1
f1n2 + f2n2 + f2n1 + f2n2 + f1n1

f2(n1 + n2) + n2(f1) + f1n1
f3(n1 + n2 + n3) + n3(f1 + f2 + f3) + f1n1 + f2n2
f4(n1 + n2 + n3 + n4) + n4(f1 + f2 + f3 + f4) + f1n1 + f2n2 + f3n3 + f4n4

f1n1  + f2n2 
n3(f1 + f2 + f3) + f3(n1 + n2 + n3) + f1n1 + f2n2

(f4 + f1)(n4 + n1) + (f4 + f2)(n4 + n2) + (f4 + f3) (n4 + n3)
f1n1 + f2n2 + f4n4 + f3n3
f4(n1 + n2 + n3 + n4) + n4(f1 + f2 + f3 + f4) + f1..4n1..4
(k + i)(nk + ni) + (k + j)(nk + nj)

(i + j)(ni + nj)
ini + jnj 2
(i + j)nk 
2knk
k(ni + nj)

