#include <bits/stdc++.h>
using namespace std;
int n;
string ori, a[9][9];
vector<int> lst;
bool check() {
	map<int, char> mp;
	for (int i = 0; i < n - 1; i++) mp[lst[i]] = ori[i];//0-L 1-K 2-V 3-E  LST = 0, 1, 2, 3
	for (int i = 1; i < n; i++) {
		for (int j = 1; j < n; j++) {
			int sum = lst[i - 1] + lst[j - 1];
			string ans;
			if (!sum) ans = mp[0];
			while (sum) {
				ans += mp[sum % (n - 1)];
				sum /= n - 1;
			}
			reverse(ans.begin(), ans.end());
			if (ans != a[i][j]) return 0;
		}
	}
	return 1;
}
int main() {
	cin >> n;
	lst.resize(n - 1);
	iota(lst.begin(), lst.end(), 0);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) 
			cin >> a[i][j];
	for (int j = 1; j < n; j++) ori += a[0][j];
	do {
		if (check()) {
			for (int i = 0; i < n - 1; i++) {
				printf("%c=%d ", ori[i], lst[i]);
			}
			cout << endl << n - 1 << endl;
			return 0;
		}
	} while (next_permutation(lst.begin(), lst.end()));
	cout << "ERROR!";
	return 0;
}
