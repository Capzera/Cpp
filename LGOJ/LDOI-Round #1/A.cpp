#include <bits/stdc++.h>
using namespace std;
int n, q, x, y;
int a[300010], sum[300010];
unordered_map<int, int> mp;
int main() {
	cin >> n >> q;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		mp[a[i]] = i;
		sum[i] = sum[i - 1];
		for (int j = 1; j < i; j++) sum[i] += a[j] > a[i];
	}
	for (int i = 1; i <= n; i++) cout << sum[i] << " ";
	cout << endl;
	while (q--) {
		cin >> x >> y;
		if (mp[x] > mp[y]) swap(x, y);
		cout << sum[mp[y]] - sum[mp[x] - 1] << endl;
	}
}

/*
4 1 3 2
0 1 1 2
1 5 2 4 3 6 1
0 0 1 2 4 4
3 2
3 1 2
1 3
2 2 
*/
