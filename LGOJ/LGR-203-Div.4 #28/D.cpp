#include <bits/stdc++.h>
using namespace std;
int n;
int main() {
	cin >> n;
	n <<= 1;
	vector<int> a(n + 1);
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) {
		if (a[a[i]] != i || a[i] == i) {
			return puts("No"), 0;
		}
	}
	cout << "Yes";
	return 0;
}
