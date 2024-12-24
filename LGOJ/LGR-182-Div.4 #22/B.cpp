#include <bits/stdc++.h>
using namespace std;
int main() {
	string s;
	cin >> s;
	sort(s.begin(), s.end());
	if (s[3] == 'D') {
		cout << "Never give up." << endl;
	} else if (s[0] == 'A' && s[3] != 'C') {
		cout << "I'm so happy." << endl;
	} else cout << "This is ok." << endl;
	return 0;
}
