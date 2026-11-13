#include <bits/stdc++.h>
using namespace std;
int x, a, b, t;
int main() {
	cin >> x >> a >> b >> t;
	if (x < 60) cout << 0;
	else if (x < 120) cout << a;
	else if (x < 240) cout << b;
	else cout << t;
	return 0;
}
