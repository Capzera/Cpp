#include <bits/stdc++.h>
using namespace std;
long long x, y, g, sum, tot = 1, lst = 1;
char ch;
vector<long long> fz, fm;
int main() {
	while (cin >> x >> ch >> y) {
		fz.push_back(x);
		fm.push_back(y);
	}
	for (int i = 0; i < fm.size(); i++) {
		tot = tot / __gcd(tot, fm[i]) * fm[i];
		sum = sum * (tot / lst) + fz[i] * (tot / fm[i]);
		g = abs(__gcd(sum, tot));
		sum /= g, tot /= g;
		lst = tot;
	}
	cout << sum;
	if (tot != 1) cout << '/' << tot;
	return 0;
}
