#include <bits/stdc++.h>
using namespace std;
long long a1, a2, b1, b2;
int main() {
	cin >> a1 >> a2 >> b1 >> b2;
	long long a[] = {a1 * b1, a1 * b2, a2 * b1, a2 * b2};
	sort(a, a + 4);
	if (a[0] >= INT_MIN && a[0] <= INT_MAX && a[3] >= INT_MIN && a[3] <= INT_MAX) {
		cout << "int";
	} else cout << "long long int";
	return 0;
}
