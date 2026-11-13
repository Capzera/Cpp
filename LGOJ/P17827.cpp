#include<bits/stdc++.h>
using namespace std;
int c1, s1, c2, s2;
char ch1, ch2;
int main() {
	// freopen("travel.in", "r", stdin);
	// freopen("travel.out", "w", stdout);
	cin >> c1 >> s1 >> ch1 >> c2 >> s2 >> ch2;
	int x = c1 == c2;
	int y = x && s1 == s2;
	int z = y && (ch1 < 'D' == ch2 < 'D');
	int w = z && (ch1 > 'C' || abs(ch1 - ch2) == 1);
	cout << x << y << z << w;
}
