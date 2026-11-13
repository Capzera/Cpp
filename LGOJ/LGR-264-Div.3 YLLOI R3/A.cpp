#include <bits/stdc++.h>
using namespace std;
long long n, m, k, ans;
int main() {
	scanf("%lld%lld%lld", &n, &m, &k);
	printf("%lld", n * (m - 1) + max(n - k, 0ll));
	return 0;
}
