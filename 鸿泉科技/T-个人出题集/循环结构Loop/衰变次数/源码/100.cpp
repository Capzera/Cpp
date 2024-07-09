#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, ans = 0;
	cin >> n;
	while (n != 1) {
		ans++;
		n /= 2;
	}
	cout << ans << endl;
	return 0;
}
