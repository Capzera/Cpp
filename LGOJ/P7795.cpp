#include <bits/stdc++.h>
using namespace std;
const int N = 3e6 + 10;
int n, k, a[N];
long long sum, head = 1, tail, q[N];
int main() {
	scanf("%d%d", &n, &k);
	for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
	for (int i = 1; i <= n; i++) {
		
	}
	return 0;
}


n 个元素的和为sum, 考虑去掉一个元素x 使得平均值变大
sum / n < (sum - x) / (n - 1)
- sum <  - n * x;
sum > n * x;
