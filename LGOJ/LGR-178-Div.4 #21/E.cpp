#include <iostream>
#include <string>
using namespace std;
int mp2, mp0, mp1, mp4;
int main() {
	int n;
	string s;
	cin >> n >> s;
	for (int i = 0; i < n; i++) {
		if (s[i] == '2') mp2++;
		else if (s[i] == '4') mp4++;
		else if (s[i] == '1') mp1++;
		else if (s[i] == '0') mp0++;
		else {
			cout << "No";
			return 0;
		}
	}
	if (mp2 == mp0 && mp2 == mp1 && mp2 == mp4) {
		cout << "Yes";
	} else {
		cout << "No";
	}
	return 0;
}
