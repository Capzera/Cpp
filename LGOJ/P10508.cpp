#include <bits/stdc++.h>
using namespace std;
long long x, cnt;
int main() {
	freopen("numlist.txt", "r", stdin);
	for (int i = 1; i <= 1000; i++) {
		cin >> x;
		while (x % 2 == 0) {
			cnt++;
			x /= 2;
		}
	}
	cout << cnt;
	return 0;
}
