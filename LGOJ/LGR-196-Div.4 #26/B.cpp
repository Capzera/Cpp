#include <bits/stdc++.h>
using namespace std;
long long w1, c1, t1, w2, c2, t2;
int main() {
	cin >> w1 >> c1 >> t1 >> w2 >> c2 >> t2;
	if (w1 != w2) {
		cout << (w1 > w2 ? 'A' : 'B');
	} else if (c1 != c2) {
		cout << (c1 > c2 ? 'A' : 'B');
	} else {
		cout << (t1 < t2 ? 'A' : 'B');
	}
	return 0;
}
