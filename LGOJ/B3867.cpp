#include <bits/stdc++.h>
using namespace std;
int n, d, a[1001], b[1001];
int main() {
	cin >> n >> d;
	for (int i = 1; i <= d; i++) cin >> a[i];
	for (int i = 1; i <= d; i++) {
		b[a[i]] += i;
	}
	for (int i = 0; i < n; i++) cout << b[i] << " ";
	return 0;
}
