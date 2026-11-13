#include <bits/stdc++.h>
using namespace std;
int n, x, top, a[101];
int main() {
	cin >> n;
	while (n--) {
		cin >> x;
		a[++top] = x;
	}
	while (top) cout << a[top--] << " ";
	return 0;
}
