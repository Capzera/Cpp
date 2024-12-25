#include <bits/stdc++.h>
using namespace std;
int n, x, a[1000010];
unordered_map<int, int> mp;
int main() {
	cin.tie(nullptr)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		mp[x] = i;
	}
	for (int i = 1; i < n; i++) {
		swap(mp[0], mp[i]);
	}
	for (int i = 0; i < n; i++) a[mp[i]] = i;
	for (int i = 1; i <= n; i++) cout << a[i] << " ";
 	return 0;
}
