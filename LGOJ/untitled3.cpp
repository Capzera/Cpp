#include <bits/stdc++.h>
using namespace std;
int n, x, cnt = 1, mp[101];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		mp[x]++;
	}
	cin >> x;
	for (int i = 100; ~i; i--) {
		if (i == x) break;
		cnt += mp[i];
	}
	cout << cnt;
	return 0;
}

