#include <bits/stdc++.h>
using namespace std;
long long t, d, t1, d1, t2, d2;
int main() {
	cin >> t >> d >> t1 >> d1 >> t2 >> d2;
	if (t1 > t2) {swap(t1, t2), swap(d1, d2);}
	if (d <= d1) cout << max(t1 - t, 0ll);
	else if (d <= d2) cout << max(t2 - t, 0ll);
	else cout << -1;
	return 0;
}
