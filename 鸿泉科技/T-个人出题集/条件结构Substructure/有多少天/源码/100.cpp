#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m;
	cin >> n >> m;
	int flag = n % 400 == 0 || n % 4 == 0 && n % 100 != 0;
	if (m == 2) {
		cout << 28 + flag;
	}
	else if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) cout << 31;
	else {
		cout << 30;
	}
	return 0;
}
