#include <bits/stdc++.h>
using namespace std;
string a, b, ans;
int l1, l2, x;
int main() {
	cin >> a >> b;
	l1 = a.size(), l2 = b.size();
	string c;
	int i = l1 - 1, j = l2 - 1;
	while (i >= 0 || j >= 0) {
		int n1 = i >= 0 ? a[i--] - '0' : 0;
		int n2 = j >= 0 ? b[j--] - '0' : 0;
		x += n1 + n2;
		c += '0' + x % 10;
		x /= 10;
	}
	if (x) c += "1";
	reverse(c.begin(), c.end());
	cout << c;
	return 0;
}
