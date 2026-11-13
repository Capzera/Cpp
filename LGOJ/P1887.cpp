#include <bits/stdc++.h>
using namespace std;
int m, n;
int main() {
	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		cout << n / m + (i > m - n % m) << " ";
	}
}
