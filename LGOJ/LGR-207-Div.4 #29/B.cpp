#include <bits/stdc++.h>
using namespace std;
int a, b, c, k;
int main() {
	cin >> a >> b >> c >> k;
	cout << (a + b >= k ? "Yes\n" : "No\n");
	cout << (a + c >= k ? "Yes\n" : "No\n");
	cout << (c + b >= k ? "Yes\n" : "No\n");
	return 0;
}
