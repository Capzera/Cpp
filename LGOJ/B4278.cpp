#include <bits/stdc++.h>
using namespace std;
int top, num, ans, stk[100001];
char op;
int main() {
	cin >> num;
	stk[++top] = num;
	while (cin >> op >> num) {
		if (op == '*') stk[top] *= num;
		else if (op == '/') stk[top] /= num;
		else if (op == '-') stk[++top] = -num;
		else stk[++top] = num;
	}
	while (top) ans += stk[top--];
	cout << ans;
	return 0;
}
