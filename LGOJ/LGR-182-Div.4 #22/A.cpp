#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	cin >> n;
	if (n >= 250) cout << n - 70;
	else if (n >= 200) cout << n - 50;
	else if (n >= 100) cout << n - 20;
	else cout << n;
	return 0;
}
