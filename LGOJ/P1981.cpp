#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int top, num, ans, stk[N];
char op;
int main() {
	cin >> num;
	stk[++top] = num;
	while (cin >> op >> num) {
		num %= 10000;
		op == '*' ? stk[top] = (stk[top] * num) % 10000 : stk[++top] = num;
	}
	while (top) ans = (ans + stk[top--]) % 10000;
	cout << ans;
	return 0;
}
