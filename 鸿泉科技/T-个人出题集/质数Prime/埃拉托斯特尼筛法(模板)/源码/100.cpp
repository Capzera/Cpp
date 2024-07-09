#include <bits/stdc++.h>
using namespace std;
bool st[1000000010];
int main() {
	long long n;
	cin >> n;
	vector<int> prime;
	for (int i = 2; i <= n; i++) {
		if (!st[i]) prime.emplace_back(i);
		for (int j = 0; j < prime.size() && i * prime[j] <= n; j++) {
			st[i * prime[j]] = 1;
			if (i % prime[j] == 0) break;
		}
	}	
	for (auto& x : prime) cout << x << " ";
	return 0;
}
