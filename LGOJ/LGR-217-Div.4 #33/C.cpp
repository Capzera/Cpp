#include <bits/stdc++.h>
using namespace std;
int n, x;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x;
		cout << x;
		if (i != n) cout << ",";
	}
	return 0;
}
