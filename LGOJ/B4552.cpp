#include <bits/stdc++.h>
using namespace std;
double n, x, ans;
int main() {
	for (int i = 1; i <= 12; i++) {
		cin >> x;
		ans += max(x - 800, 0.0) * 0.2;
	}
	printf("%.2lf", ans);
	return 0;
}
