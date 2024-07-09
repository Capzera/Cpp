#include <bits/stdc++.h>
using namespace std;
long long st[2000000];
int main() {
	int n;
	cin >> n;
	vector<long long> prime;
	for (int i = 2; prime.size() < n; i++) {
		if (!st[i]) prime.emplace_back(i);
		for (int j = 0; j < prime.size() && i * prime[j] <= 2000000; j++) {
			st[i * prime[j]] = 1;
			if (i % prime[j] == 0) break;
		}
	}
	cout << prime.back() << endl;
	return 0;
}
