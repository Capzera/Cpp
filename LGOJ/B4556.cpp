#include <bits/stdc++.h>
using namespace std;
int n;
string s;
int main() {
	cin >> n >> s;
	for (auto& x : s) {
		if (isupper(x)) x = tolower(x);
		else if (islower(x)) x = toupper(x);
		else x = '*';
	}
	cout << s;
	return 0;
}
