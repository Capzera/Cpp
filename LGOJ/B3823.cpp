#include <bits/stdc++.h>
using namespace std;
long long a, b, k, r, c;
int main() {
	cin >> a >> b >> k >> r >> c;
	printf("%lld", (b - a) * k * (r - c));
	return 0;
}
