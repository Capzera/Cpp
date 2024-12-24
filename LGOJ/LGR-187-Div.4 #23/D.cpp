#include <bits/stdc++.h>
using namespace std;
long long n, c1, c2;
int main() {
	cin >> n;
	vector<int> a(n), b(n);
	for (auto& x : a) cin >> x;
	for (auto& x : b) cin >> x;
	for (int i = 0; i < n; i++) {
		c1 += a[i] > b[i];
		c2 += a[i] < b[i];
	}
	cout << c1 << " " << c2 << endl;
	cout << (c1 > c2 ? "S" : (c1 < c2 ? "Q" : "Tie"));
	return 0;
}
