#include <bits/stdc++.h>
using namespace std;
long long n, m, x, f = 1;
int get(int x) {
	int cnt = 0;
	while (x) {
		cnt += x % 10;
		x /= 10;
	}
	return cnt;
}
int main() {
	cin >> n >> m;
	while (m--) {
		cin >> x;
		if (x % n && get(x) % n == 0) {
			cout << x << endl;
			f = 0;
		}
	}
	if (f) puts("None");
	return 0;
}
