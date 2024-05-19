#include <bits/stdc++.h>
using namespace std;
long long n, m, p, loc, mx;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n >> m >> p;
	vector<long long> a(n + 1);
	vector<int> idx, ans, h(n + 1);
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
	}
	iota(h.begin(), h.end(), 0);
	sort(h.begin() + 1, h.end(), [&](auto & x, auto & y) {
		return a[x] < a[y];
	});
	for (int i = 1; i <= n; i++) {
		if (h[i] == p) {
			loc = i;
			break;
		}
	}
	for (int i = 1, j = 0; i <= n; i++) {
		int bonus = 0, sumBonus = 0;
		if (h[i] <= loc) {
			sumBonus = 2ll * (a[p] - a[h[i]]);
			if (a[h[i]] > 0 || a[p] < 0) sumBonus += 2ll * min(abs(a[h[i]]), a[p]);
			if (sumBonus <= m + 5) bonus = 5;
		}
		if (!bonus && 2ll * abs(a[h[i]]) > m) continue;
		if (i == loc + 1) j = i;
		j = max(i, j);
		while (j <= n) {
			if (a[h[j]] <= 0) j++;
			else if (a[h[i]] >= 0 && 2ll * a[h[j]] <= m + bonus) j++;
			else if (a[h[i]] <= 0 && a[h[j]] >= 0 && 2ll * (a[h[j]] - a[h[i]]) <= m + bonus) j++;
			else break;
		}
		if (j - i == mx) idx.emplace_back(i);
		if (j - i > mx) {
			idx.clear();
			idx.push_back(i);
			mx = j - i;
		}
	}
	cout << mx << endl;
	for (auto& st : idx) {
		vector<int> tmp(mx);
		for (int j = 0; j < mx; j++) {
			tmp[j] = h[st + j];
		}
		sort(tmp.begin(), tmp.end());
		if (ans.empty() || tmp < ans) ans = tmp;
	}
	for (auto& x : ans) cout << x << " ";
	return 0;
}
/*
10 7 10
3 8 2 -11 -5 10 14 15 16 4
*/
