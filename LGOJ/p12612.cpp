#include <bits/stdc++.h>
using namespace std;
int n, ans, cnt, a[500010];
char x;
int main() {
	cin >> n;
	for (int i = 1, j = 1; i <= n; i++) {
		cin >> x;
		a[i] = x == 'S';
		if (!a[i]) cnt++;
		while (j < i && cnt == 2) {
			if (!a[j++]) cnt--;
		}
		ans = max(ans, i - j + 1);
	}
	if (ans == n && !cnt) ans--;
	cout << ans;
	return 0;
}
