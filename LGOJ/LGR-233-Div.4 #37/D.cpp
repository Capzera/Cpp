#include <bits/stdc++.h>
#define int long long
using namespace std;
int n, k;
signed main() {
	cin >> n >> k;
	vector<int> a(n), vec;
	for (auto& x : a) cin >> x;
	int i = 0, j = k;
	while (i < k && j < n) vec.push_back(i & 1 ? a[j++] : a[i++]);
	while (i < k) vec.push_back(a[i++]);
	while (j < n) vec.push_back(a[j++]);
	for (auto& x : vec) cout << x << " ";
	return 0;
}

