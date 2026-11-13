#include <bits/stdc++.h>
using namespace std;
const int N = 130;
struct node {
	int val, id;
	bool operator < (const struct node& rhs) const {
		return val < rhs.val;
	}
} a[N];
int n, m;
int main() {
	cin >> n;
	m = 1 << n;
	for (int i = 1; i <= m; i++) {
		cin >> a[i].val;
		a[i].id = i;
	}
	int j = 1;
	while (--n) {
		for (int i = 1; i <= m; i += j << 1) {
			if (a[i] < a[i + j]) swap(a[i], a[i + j]);
		}
		j <<= 1;
	}
	cout << (a[1] < a[j + 1] ? a[1].id : a[j + 1].id) << endl;
	return 0;
}
