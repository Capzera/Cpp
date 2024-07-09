#include <bits/stdc++.h>
using namespace std;
int main() {
	for (int i = 1; i < 10; i++) {
		for (int j = 1; j <= i; j++) {
			cout << i << " * " << j << " = ";
			int x = i * j;
			string ans = "";
			while (x) {
				ans += x % 8 + '0';
				x /= 8;
			}
			reverse(ans.begin(), ans.end());
			cout << ans << " ";
		}
		cout << endl;
	}
	return 0;
}
