#include <bits/stdc++.h>
using namespace std;
long long a, b;
int main() {
	cin >> a >> b;
	if (a >= 8 && b <= 12) return puts("M"), 0;
	if (a >= 13 && b <= 17) return puts("A"), 0;
	if (a >= 18 && b <= 21) return puts("E"), 0;
	puts("F");
	return 0;
}
