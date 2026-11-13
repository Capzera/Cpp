#include <bits/stdc++.h>
using namespace std;
int n, x, cnt[2];
int main() {
	cin >> n;
	while (n--) {
		cin >> x;
		putchar('A' + (cnt[0] > cnt[1]));
		cnt[cnt[0] > cnt[1]] += x;
	}
	return 0;
}
