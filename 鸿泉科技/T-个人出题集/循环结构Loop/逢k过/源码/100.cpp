#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m;
	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		if (i % m == 0) continue;
		cout << i << " ";
	}
	return 0;
}
