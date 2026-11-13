#include <bits/stdc++.h>
using namespace std;
long long a, b, ans = 1;
int main() {
	cin >> a >> b;
	if (a == 1) return puts("1"), 0;
	while (b--) {
		ans = ans * a;
		if (ans > 1e9) return puts("-1"), 0;
	}
	cout << ans;
	return 0;
}

