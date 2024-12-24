#include <bits/stdc++.h>
using namespace std;
int n, m, x;
int mp[1000][1001];
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		for (int j = 0; j < n / m; j++) {
			if (mp[j][x]) continue;
			mp[j][x] = i;
			break;
		}
	}
	for (int i = 0; i < n / m; i++) {
		for (int j = 1; j <= m; j++) cout << mp[i][j] << endl;
	}
	return 0;
}
