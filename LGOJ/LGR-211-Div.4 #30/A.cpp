#include <bits/stdc++.h>
using namespace std;
long long a, b, c, d, x, ans;
int main() {
	cin >> a >> b >> c >> d >> x;
	cout << (x >= a) * b + (x >= c) * d;
	return 0;
}
