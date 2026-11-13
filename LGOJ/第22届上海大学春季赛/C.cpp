#include <bits/stdc++.h>
using namespace std;
double pi = atan(1) * 4;
double cot(double x) {
	double ans = 1.0 / tan(x);
	return ans;
}
void solve() {
	double x;
	cin >> x;
	double ans = x - 0.5 + atan(cot(pi * x))/pi;
	cout << fixed << setprecision(6) << ans << endl;
}
int main() {
	int t;
	cin >> t;
	while (t--) {
		solve();
	}
	return 0;
}
