#include <bits/stdc++.h>
using namespace std;
int n, a, b, flag;
int main() {
	cin >> n >> a >> b;
	if (a - b >= 0 && a - b <= n) {
		cout << a - b << " ";
		flag++;
	}
	if (a + b >= 0 && a + b <= n && b) {
		cout << a + b;
		flag++;
	}
	if (!flag) cout << "No solution";
	return 0;
}
