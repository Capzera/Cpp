#include <bits/stdc++.h>
using namespace std;
const int N = 5e4 + 10;
struct cow {
	int x, id;
	bool operator < (const struct cow& rhs) const {
		return x < rhs.x;
	}
} a[N];
int n, ans = INT_MAX;
set<int> mp;
map<int, int> slide;
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> a[i].x >> a[i].id;
		mp.insert(a[i].id);
	}
	sort(a + 1, a + n + 1);
	int len = mp.size();
	for (int i = 1, j = 1; i <= n; i++) {
		slide[a[i].id]++;
		while (j < i && slide.size() == len) {
			ans = min(ans, a[i].x - a[j].x);
			slide[a[j].id]--;
			if (!slide[a[j].id]) slide.erase(a[j].id);
			j++;
		}
	}
	cout << ans;
	return 0;
}
