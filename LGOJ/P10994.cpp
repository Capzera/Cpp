#include <bits/stdc++.h>
using namespace std;
int stu, tea, cs, ct;
string s;
int main() {
	cin >> stu >> tea >> s;
	int n = s.size();
	if (stu + tea > n) return puts("-1"), 0;
	for (auto x : s) cs += x == 'S';
	ct = n - cs;
	cout << max({stu - cs, tea - ct, 0});
	return 0;
}
