#include <bits/stdc++.h>
using namespace std;
int n[3], x;
unordered_set<int> s[4];
int main() {
	for (int i = 0; i < 3; i++) cin >> n[i];
	for (int i = 0; i < 3; i++) {
		while (n[i]--) {
			cin >> x;
			s[i].insert(x);
			s[3].insert(x);
		}
	}
	for (int i = 0; i < 4; i++) cout << s[i].size() << " ";
	return 0;
}
