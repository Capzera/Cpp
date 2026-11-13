#include <bits/stdc++.h>
using namespace std;
string num1[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
string num2[] = {"", "ten", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};
string num3[] = {"", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "ninteen"};
int n, a[4];
int main() {
	cin >> n;
	if (!n) return puts("zero"), 0;
	a[0] = n / 1000, n %= 1000;
	a[1] = n / 100, n %= 100;
	a[2] = n / 10, a[3] = n % 10;
	if (a[0]) cout << num1[a[0]] << " thousand ";
	if (a[1]) cout << num1[a[1]] << " hundred ";
	if (a[2]) {
		if (a[0] && !a[1]) cout << "and ";
		if (a[2] > 1 || a[2] && !a[3]) cout << num2[a[2]] << " ";
	}
	if (a[3]) {
		if (!a[2] && (a[1] || a[0])) cout << "and ";
		cout << (a[2] == 1 ? num3[a[3]] : num1[a[3]]);
	}
	return 0;
}
