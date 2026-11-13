#include <bits/stdc++.h>
using namespace std;
long long n, x;
string ans;
void op(int k) {
	swap(ans[k], ans[x]);
	swap(ans[k + 1], ans[x + 1]);
	x = k;
	cout << ans << endl;
}
void f(int n) {
	if (n == 4) {
		op(3), op(7), op(1), op(6), op(0);
		return;
	}
	op(n - 1), op(2 * n - 2), f(n - 1);
}
int main() {
	cin >> n;
	ans.resize(2 * n + 2, '-');
	x = n << 1;
	for (int i = 0; i < n; i++) ans[i] = 'o';
	for (int i = n; i < 2 * n; i++) ans[i] = '*';
	cout << ans << endl;
	f(n);
	return 0;
}
/*
ooooooo*******--

oooooo--******o*
oooooo******--o*

ooooo--*****o*o*
ooooo*****--o*o*

oooo--****o*o*o*
oooo****--o*o*o*

ooo--***o*o*o*o*

ooo*o**--*o*o*o*
o--*o**oo*o*o*o*
o*o*o*--o*o*o*o*
--o*o*o*o*o*o*o*
*/
