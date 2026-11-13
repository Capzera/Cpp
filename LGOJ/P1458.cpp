#include <bits/stdc++.h>
using namespace std;
long long n, k = 1;
struct frac {
	int a, b;
	bool operator < (const struct frac& rhs) {
		return a * rhs.b < b * rhs.a;
	}
	bool operator == (const struct frac& rhs) {
		return a * rhs.b == b * rhs.a;
	}
} a[25610];
int main() {
	cin >> n;
	a[0].a = 0, a[0].b = 1;
	for (int y = 1; y <= n; y++) {
		for (int x = 1; x <= y; x++) {
			int g = __gcd(x, y);
			a[k].a = x / g, a[k].b = y / g;
			k++;
		}
	}
	sort(a, a + k);
	for (int i = 0; i < k; i++) {
		if (i && a[i] == a[i - 1]) continue;
		cout << a[i].a << "/" << a[i].b << endl;
	}
	return 0;
}
