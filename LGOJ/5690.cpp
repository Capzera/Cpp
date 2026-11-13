#include <bits/stdc++.h>
using namespace std;
int m, d, ans, month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
char ch;
int main() {
	cin >> m >> ch >> d;
	if (!m || m > 12) {
		ans++, m %= 10;
		if (m == 2) m = 12;
		if (!m) m = 10;
	}
	if (!d || d > month[m]) ans++;
	cout << ans;
	return 0;
}


