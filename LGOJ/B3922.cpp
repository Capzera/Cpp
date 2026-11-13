#include <bits/stdc++.h>
using namespace std;
int n, m;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		if (i % m == 0) continue;
		cout << i << endl;
	}
	return 0;
}
