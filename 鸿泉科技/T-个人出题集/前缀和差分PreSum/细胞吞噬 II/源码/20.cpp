#include <bits/stdc++.h>
using namespace std;
long long n, k, ans1, ans2;
int a[200005];
int main() {
	cin >> n >> k;
	for (int i = 1; i <= n; i++) cin >> a[i];
	for (int i = 1; i <= n - k; i++) {
		long long sum = 0;
		for (int j = 0; j <= k; j++) {
			sum += a[i + j];
		}
		if (sum > ans1) {
			for (int j = i; j <= i + k; j++) {
				long long sum2 = a[j], flag = 0;
				int l = j - 1, r = j + 1;
				while (l >= i || r <= i + k) {
					
					if (sum2 >= a[l]) {
						sum2 += a[l--];
					} else if (sum2 >= a[r]) {
						sum2 += a[r++];
					} else {
						flag = 1;
						break;
					}
				}
				
			}
		}
	}
}
