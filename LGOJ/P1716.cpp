#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, k = 0;
	cin >> n;
	vector<int> a(n);
	for (auto& x : a) cin >> x;
	sort(a.begin(), a.end());
	int i = 0, j = n - 1;
	while (i < j) {
		if (!k) cout << a[j--] << endl;
		else cout << a[i++] << endl;
		k ^= 1;
	}
	return 0;
}
