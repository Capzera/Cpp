#include <bits/stdc++.h>
using namespace std;
long long n, k, a, b, ans;
int main() {
	cin >> n >> a >> k >> b;
	if (n % 2) {
		cout << -1;
		return 0;
	}
	n /= 2;
	if (b <= a * k) { // 比较一车一车运和手动搬运的更好的那个
		//一车一车运更好
		ans += n / k * b;  // 算出用车需要多少体力
		int rest = n % k; // 还剩下多少桶水泥
		if (rest * a < b) {  // 手搬 < 用车
			ans += rest * a;
		} else {  // 手搬 >= 用车
			ans += b;
		}
	} else {
		// 手动搬运更好
		ans = n * a;
	}
	cout << ans * 2; // 答案最终 * 2
	return 0;
}
/*
k
a * k     b
*/
