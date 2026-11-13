#include <bits/stdc++.h>
using namespace std;
long long a, b, n;
int main() {
	cin >> a >> b;
	n = sqrt(a + b << 1);
	if (n * (n + 1) != a + b << 1) return puts("No"), 0;
	cout << n;
	for (int i = n; i > 0; i--) {
		if (a > b) a -= i, cout << " " << i;
		else b -= i;
	}
	return 0;
}
