#include <bits/stdc++.h>
using namespace std;
int n, m, k;
string s;
int main() {
	cin >> n >> m >> k;
	while (n--) {
		cin >> s;
		string t;
		for (int i = 0; i < m; i++)
			for (int d = 0; d < k; d++) 
				t += s[i];
		for (int i = 0; i < k; i++) cout << t << endl;
	}
	return 0;
}
