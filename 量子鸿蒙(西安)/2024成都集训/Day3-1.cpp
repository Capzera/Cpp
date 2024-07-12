#include<bits/stdc++.h>
using namespace std;
bool isPrime(long long x) {
	if (x == 1) return 0;
	if (x == 2 || x == 3) return 1;
	if (x % 6 != 1 && x % 6 != 5) return 0;
	long long n = sqrt(x); // 降低算法复杂度
	for (int i = 5; i <= n; i += 6) {
		if (x % i == 0 || x % (i + 2) == 0)return 0;
	}
	return 1;
}// 六倍定理
int main() {
	long long a, b;
	cin >> a >> b;
	cout << (isPrime(a) ? "Yes" : "No") << endl;
	cout << (isPrime(b) ? "Yes" : "No") << endl;
	return 0;
}
/*
T(2max(a, b) - 4)
n = max(a, b)
T(2n - 4)
时间复杂度  O(n)
O(10^18)
1S = 3 x 10^8

时间复杂度O(n ^ 0.5)  --- 10 ^ 9

*/
