#include <bits/stdc++.h>
using namespace std;
long long t, n, k;
int main() {
	cin >> t;
	while (t--) {
		cin >> n >> k;
		if (!k) {
			cout << 1 << endl;
			continue;
		}
		cout << n / k - n / (k + 1) << endl;
	}
	return 0;
}
/*

100 4
20 25
21 22 23 24 25
*/
