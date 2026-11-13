#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 10;
long long n, top, ans, a[N];
pair<int, int> stk[N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n; i++) {
		pair<int, int> p = {a[i], 1};
		while (top && a[i] >= stk[top].first) {
			ans += stk[top].second;
			if (a[i] == stk[top].first) p.second += stk[top].second;
			top--;
		}
		if (top) ans++;
		stk[++top] = p;
	}
	cout << ans;
	return 0;
}
