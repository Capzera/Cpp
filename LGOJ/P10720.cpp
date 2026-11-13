#include <bits/stdc++.h>
using namespace std;
bool st[1000010];
vector<int> prime;
void sieve() {
	for (int i = 2; i <= 1e6; i++) {
		if (!st[i]) prime.push_back(i);
		for (int j = 0; j < prime.size() && i * prime[j] <= 1e6; j++) {
			st[i * prime[j]] = 1;
			if (i % prime[j] == 0) break;
		}
	}
}
bool check(int n) {
	int cnt = 0;
	for (auto& p : prime) {
		if (n % p != 0) continue;
		if (++cnt > 2) return 0;
		while (n % p == 0) n /= p;
	}
	return cnt == 2;
}
int n, x;
int main() {
	cin >> n;
	sieve();
	while (n--) {
		cin >> x;
		cout << check(x) << endl;
	}
	return 0;
}
