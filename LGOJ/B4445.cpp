#include <bits/stdc++.h>
using namespace std;
double a, b, c, d, a1, a2;
int main() {
	cin >> a >> b >> c >> d;
	a1 = a * 0.5;
	a2 = b >= 300 ? d : c;
	cout << fixed << setprecision(1) << min(a1, a2);
	return 0;
}
