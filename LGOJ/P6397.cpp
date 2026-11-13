#include <bits/stdc++.h>
using namespace std;
double k, a[100010];
int n;
bool check(double x) {
	int lst = a[1] + x;
	for (int i = 2; i <= n; i++) {
		if (a[i] - x - lst > k) return 0;
		lst += k;
		
	}
	return 1;
}
int main() {
	cin >> k >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i]; 
	}
	double l = 0, r = a[n];
	while (r - l >= 1e-6) {
		double mid = (l + r) / 2.0;
		if (check(mid)) r = mid;
		else l = mid;
	}
	return 0;
}
