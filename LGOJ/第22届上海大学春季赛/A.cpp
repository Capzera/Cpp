#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m, rest, k = 1, group;
	string s;
	cin >> n >> s;
	m = 2 * n - 2;
	rest = s.size() % m;
	group = s.size() / m;
	vector<int> cnt(n, group * 2);
	cnt[0] = cnt[n - 1] = group;
	int i = 0;
	while (rest) {
		cnt[i]++;
		rest--;
		if (i == n - 1) k = -1;
		i += k;
	}
	vector<string> a(n);
	vector<int> p(n, 0);
	for (int i = 0, j = 0; i < n; i++) {
		a[i] = s.substr(j, cnt[i]);
		j += cnt[i];
	}
	for (int i = 0, j = 0, k = -1; i < s.size(); i++) {
		cout << a[j][p[j]++];
		if (j == 0 || j == n - 1) {
			k = k == -1 ? 1 : -1;
		}
		j += k;
	}
	return 0;
}
