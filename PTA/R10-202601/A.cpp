#include <bits/stdc++.h>
using namespace std;
string s, num[2];
long long n, a[2];
int main() {
	getline(cin, s);
	n = s.size();
	for (int i = 0, k = 0; i < n; i++) {
		if (s[i] == '+') k++;
		if (!isdigit(s[i])) continue;
		num[k] += s[i];
	}
	for (int i = 0; i < 2; i++) {
		if (num[i].size() > 10 || num[i].size() == 10 && num[i] > "1000000000") return puts("Too Big"), 0;
		reverse(num[i].begin(), num[i].end());
		while (num[i].size() > 1 && num[i].back() == '0') num[i].pop_back();
		reverse(num[i].begin(), num[i].end());
		a[i] = stoll(num[i]);
	}
	if (a[0] + a[1] > 1000000000) return puts("Too Big"), 0;
	cout << a[0] + a[1];
	return 0;
}
