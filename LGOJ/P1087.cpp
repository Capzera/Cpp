#include <bits/stdc++.h>
using namespace std;
int n;
char x;
char fun(int n) {
	char ans;
	if (!n) {
		x = getchar();
		ans = x == '1' ? 'I' : 'B';
		return ans;
	}
	char ch1 = fun(n - 1), ch2 = fun(n - 1);
	ans = ch1 == ch2 ? ch1 : 'F';
	cout << ans;
	return ans;
}
int main() {
	cin >> n;
	fun(n);
	return 0;
}
