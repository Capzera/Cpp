#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, k;
	cin >> n >> k;
	n += k;
	cout << n << " ";
	if (n > 89) cout << 'A';
	else if (n > 74) cout << 'B';
	else if (n > 59) cout << 'C';
	else cout << 'D';
	return 0;
}
