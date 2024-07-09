/*
买书 数学-贪心
*/
#include <bits/stdc++.h>
using namespace std;
int main() {
	int a, b, s;
	cin >> a >> b >> s;
	cout << min(abs(s - a), abs(s - b)) + abs(a - b);
	return 0;
}
