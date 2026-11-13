#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 10;
int n, k;
struct goods {
	long long tot, id, expen, cheap;
	vector<int> price;
	void read() {
		price.resize(k);
		for (auto& x : price) cin >> x, tot += x;
		sort(price.begin(), price.end());
		expen = price.back(), cheap = price[0];
	}
	bool operator < (const struct goods& rhs) const {
		if (tot != rhs.tot) return tot < rhs.tot;
		if (expen != rhs.expen) return expen < rhs.expen;
		if (cheap != rhs.cheap) return cheap < rhs.cheap;
		return id < rhs.id;
	}
} a[N];
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) a[i].read(), a[i].id = i;
	sort(a + 1, a + n + 1);
	for (int i = 1; i <= n; i++) cout << a[i].id << " ";
	return 0;
}
