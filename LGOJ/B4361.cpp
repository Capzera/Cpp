#include <bits/stdc++.h>
using namespace std;
struct stu {
	int h, w;
	bool operator > (const struct stu& rhs) const {
		if (h != rhs.h) return h < rhs.h;
		return w < rhs.w;
	}
	void read() {cin >> h >> w;}
} a[3001];
int n, ans;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) a[i].read();
	for (int i = 1; i < n; i++) {
		int f = 1;
		for (int j = 1; j <= n - i; j++) {
			if (a[j] > a[j + 1]) {
				f = 0;
				ans++;
				swap(a[j], a[j + 1]);
			}
		}
		if (f) break;
	}
	cout << ans;
	return 0;
}
