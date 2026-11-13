#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
struct n1 {
	long long p, c;
	bool operator < (const struct n1& rhs) const {
		return p < rhs.p;
	}
} point[N];
struct n2 {
	long long a, b;
	bool operator < (const struct n2& rhs) const {
		return a - b > rhs.a - rhs.b;
	}
} car[N];
long long n, m, k, ans;
int main() {
	cin >> n >> m >> k;
	for (int i = 1; i <= n; i++) {
		cin >> point[i].p >> point[i].c;
	}
	for (int i = 1; i <= m; i++) {
		cin >> car[i].a >> car[i].b;
	}
	sort(point + 1, point + n + 1);
	sort(car + 1, car + m + 1);
	for (int i = 1, j = 1; car[i].a >= car[i].b; i++) {
		ans += car[i].a * point[j].p + car[i].b * (k - point[j].p);
		if (--point[j].c == 0) j++;
	}
	for (int i = m, j = n; car[i].a < car[i].b; i--) {
		ans += car[i].a * point[j].p + car[i].b * (k - point[j].p);
		if (--point[j].c == 0) j--;
	}
	cout << ans * 2;
	return 0;
}
