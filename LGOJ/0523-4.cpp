#include <bits/stdc++.h>
using namespace std;
int l, e;
int main() {
	cin >> l >> e;
	if (l < 3) {
		cout << "Impossible";
	} else if (l >= 8 || l >= 6 && e == 1) {
		cout << 3;
	} else {
		cout << 6;
	}
	return 0;
}

