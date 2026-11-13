#include <bits/stdc++.h>
using namespace std;
struct node {
	int st, ed;
	bool operator < (const struct node& rhs) const {
		return st < rhs.st;
	}
} a[50001];
int n;
int main() {
	cin >> n;
	for (int i = 0; i < n; i++) cin >> a[i].st >> a[i].ed;
	sort(a, a + n);
	int st = a[0].st, ed = a[0].ed;
	for (int i = 1; i < n; i++) {
		if (a[i].st > ed) {
			cout << st << " " << ed << endl;
			st = a[i].st, ed = a[i].ed;
		} else ed = max(ed, a[i].ed);
	}
	cout << st << " " << ed;
	return 0;
}
