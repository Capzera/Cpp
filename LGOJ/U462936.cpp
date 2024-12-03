#include<bits/stdc++.h>
using namespace std;
string s;
int n, k;
int main() {
	cin >> s >> k;
	n = s.size();
	while (k--) {
		string backup = s;
		for (int i = 1; i < s.size() - 1; i++) {
			if (s[i] <= s[i + 1] && s[i] <= s[i - 1]) {
				s.erase(i, 1);
				break;
			}
			if (i == s.size() - 2 && s[i + 1] <= s[i]) s.erase(i + 1, 1);
		}
		if (backup == s) break;
		cout << s << endl;
	}
	cout << s;
	return 0;
}
/*
6799995393187825035763159254384425526090780052267671052685
679995393187825035763159254384425526090780052267671052685
679999593187825035763159254384425526090780052267671052685
*/
