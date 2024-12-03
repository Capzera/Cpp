#include <bits/stdc++.h>
using namespace std;
int n, x, a[8], ans[8];
int main() {
	cin >> n;
	for (int i = 1; i <= 7; i++) {
		cin >> a[i];
	}
	while (n--) {
		int cnt = 0;
		for (int i = 1; i <= 7; i++) {
			cin >> x;
			for (int j = 1; j <= 7; j++) 
				if (x == a[j]) cnt++;
		}
		ans[8 - cnt]++;
	}
	for (int i = 1; i <= 7; i++) cout << ans[i] << " ";
	return 0;
}
