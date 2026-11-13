#include <bits/stdc++.h>
using namespace std;
long long a[5], b[5], ans;
int main() {
	for (int i = 1; i <= 4; i++) cin >> a[i];
	for (int i = 1; i <= 4; i++) cin >> b[i];
	for (int i = 1; i <= 3; i++)
		for (int j = i + 1; j <= 4; j++)
			ans += a[i] < a[j] ^ b[i] < b[j];
	cout << ans;
	return 0;
}
