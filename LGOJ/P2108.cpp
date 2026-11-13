#includ <bits/stdc++.h>
using namespace std;
string s;
long long num, now, f;
int main() {
	f = 1;
	while (cin >> s) {
		if (s == "negative")	f = -1;
		if (s == "one")	now++;
		if (s == "two") now += 2;
		if (s == "three") now += 3;
		if (s == "four") now += 4;
		if (s == "five") now += 5;
		if (s == "six")	now += 6;
		if (s == "seven") now += 7;
		if (s == "eight") now += 8;
		if (s == "nine") now += 9;
		if (s == "ten")	now += 10;
		if (s == "eleven") now += 11;
		if (s == "twelve") now += 12;
		if (s == "thirteen") now += 13;
		if (s == "fourteen") now += 14;
		if (s == "fifteen")	now += 15;
		if (s == "sixteen")	now += 16;
		if (s == "seventeen") now += 17;
		if (s == "eighteen") now += 18;
		if (s == "nineteen") now += 19;
		if (s == "twenty") now += 20;
		if (s == "thirty") now += 30;
		if (s == "forty") now += 40;
		if (s == "fifty") now += 50;
		if (s == "sixty") now += 60;
		if (s == "seventy")	now += 70;
		if (s == "eighty") now += 80;
		if (s == "ninety") now += 90;
		if (s == "hundred")	now *= 100;
		if (s == "thousand") num += now * 1000, now = 0;
		if (s == "million")	num += now * 1000000, now = 0;
	}
	num += now;
	cout << num * f;
}
