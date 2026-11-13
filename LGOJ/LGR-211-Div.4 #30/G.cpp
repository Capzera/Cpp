#include <bits/stdc++.h>
using namespace std;
long long n, m;
int main() {
	cin >> n;
	vector<string>name(n);
	for (auto& x : name) cin >> x;
	cin >> m;
	vector<string> word(m);
	for (auto& x : word) cin >> x;
	for (auto& x : name) {
		bool f = 0;
		for (auto& y : word) {
			if (x.find(y) != -1) {
				f = 1;
				break;
			}
		}
		cout << (f ? "Yes\n" : "No\n");
	}
	return 0;
}
