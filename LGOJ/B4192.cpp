#include <bits/stdc++.h>
using namespace std;
long long n, x, y, pre, sum, a[500001];
bool check(int p) {
	return p >= x && p <= y;
}
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		sum += a[i];
	}
	cin >> x >> y;
	for (int i = 1; i <= n; i++) {
		pre += a[i];
		// 分数小于 i + 1 的有pre个，大于等于i + 1的有sum - pre个
		if (check(pre) && check(sum - pre)) {
			cout << i + 1;
			return 0;
		}
	}
	return puts("0"), 0;
}
