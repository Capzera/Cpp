#include <bits/stdc++.h>
using namespace std;
long long n, k;
bool solve() {
	cin >> n >> k;
	return n >= k * (k + 1) / 2;
}
int main() {
	int t;
	cin >> t;
	while (t--) {
		cout << (solve() ? "Yes" : "No") << endl;
	}
	return 0;
}
/*

*/
