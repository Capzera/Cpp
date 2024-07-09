#include <bits/stdc++.h>
using namespace std;
int a[200005];
int main() {
	long long n, k, l, r, maxAns = 0, sum = 0, ans, slide = 0;
	cin >> n >> k;
	for (int i = 1, j = 1; i <= n; i++) {
		cin >> a[i];
		slide += a[i];
		if (i > k) {
			if (slide > maxAns) {
				maxAns = slide;
				l = j, r = i;
				ans = l;
				sum = 0;
				for (; l < r; l++) {
					sum += a[l];
					if (sum < a[l + 1]) {
						ans = l + 1;
					}
				}
			}
			slide -= a[j++];
			
		} 
	}
	cout << maxAns << " " << ans;
}
