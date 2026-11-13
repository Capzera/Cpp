#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
struct node {
	long long loc, dir, id;
	bool operator < (const struct node& rhs) const {
		return loc < rhs.loc;
	}
} a[N];
long long n, t, vec[N];
int main() {
	cin >> n >> t;
	iota(vec + 1, vec + n + 1, 1);
	for (int i = 1; i <= n; i++) {
		cin >> a[i].loc >> a[i].dir;
		a[i].id = i;
	}
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) {
		vec[a[i].id] = i;
		a[i].loc += t * a[i].dir;
	}
	sort(a + 1, a + n + 1);
	for (int i = 1; i < n; i++) {
		if (a[i].loc == a[i + 1].loc) {
			a[i].dir = a[i + 1].dir = 0;
		}
	}
	for (int i = 1; i <= n; i++) {
		cout << a[vec[i]].loc << " " << a[vec[i]].dir << '\n';
	}
	return 0;
}
/*
t = 1
2 0  1 1
6 1  2 3
2 0  3 2
11 1 4 4
*/
