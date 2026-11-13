#include <bits/stdc++.h>
using namespace std;
int st[1000001];
vector<int> prime;
int n;
int main() {
	cin >> n;
	for (int i = 2; i <= n; i++) {
		if (!st[i]) prime.push_back(i);
		for (int j = 0; i * prime[j] <= n && j < prime.size(); j++) {
			st[i * prime[j]] = 1;
			if (i % prime[j] == 0) break; 
		}
	}
	cout << prime.size() + 1 << endl;
	return 0;
}

