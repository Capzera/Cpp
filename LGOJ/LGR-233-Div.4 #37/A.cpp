#include <bits/stdc++.h>
#define int long long
using namespace std;
long long n, m;

void solve() {
	cin >> n >> m;
	if (n > 0 && m > 0) cout << "NorthEast\n";
	if (n > 0 && m < 0) cout << "NorthWest\n";
	if (n < 0 && m > 0) cout << "SouthEast\n";
	if (n < 0 && m < 0) cout << "SouthWest\n"; 
}

signed main() {
	solve();
	return 0;
}
