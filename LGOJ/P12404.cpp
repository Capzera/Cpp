#include <bits/stdc++.h>
using namespace std;
long long n, t, x;
int main() {
	cin >> n >> t >> x;
	cout << min(n, 2ll * t * x) << " " << max(2, x);
	return 0;
}
