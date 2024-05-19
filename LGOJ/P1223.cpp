#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	long long sum = 0, ans = 0;
	cin >> n;
	vector<int> a(n), h(n);
	for (auto& x : a) cin >> x;
	iota(h.begin(), h.end(), 0);
	sort(h.begin(), h.end(), [&](auto x, auto y){
		return a[x] < a[y];
	});
	for (auto& x : h) {
		cout << x + 1 << " ";
		ans += sum;
		sum += a[x];
	}
	double ret = 1.0 * ans / n;
	printf("\n%.2f", ret);
	return 0;
}
