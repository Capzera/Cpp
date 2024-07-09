#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m, x, cnt = 0;
	cin >> n >> m;
	for (int i = 0; i < m; i++) {
		cin >> x;
		if (n < x) continue;
		n -= x;
		cnt++;
	}
	cout << m - cnt << endl;
	return 0;
}
