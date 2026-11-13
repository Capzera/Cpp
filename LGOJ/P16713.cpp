#include <bits/stdc++.h>
using namespace std;
int l1, r1, l2, r2, st[200001];
vector<int> prime;
void sieve(int n) {
	for (int i = 2; i <= n; i++) {
		if (!st[i]) prime.push_back(i);
		st[i] += st[i - 1];
		for (int j = 0; j < prime.size() && i * prime[j] <= n; j++) {
			st[i * prime[j]] = 1;
			if (i % prime[j] == 0) break;
		}
	}
}
int main() {
	cin >> l1 >> r1 >> l2 >> r2;
	sieve(r1 + r2);
	for (int x = l1; x <= r1; x++) {
		int l = x + l2, r = x + r2;
		if (st[r] - st[l - 1] != r - l + 1) continue;
		return puts("Burnside"), 0;
	}
	puts("Edisnrub");
	return 0;
}
