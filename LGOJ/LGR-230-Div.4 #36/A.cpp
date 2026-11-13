#include <bits/stdc++.h>
using namespace std;
int t;
int main() {
	cin >> t;
	printf("|%d|%d|:|%d|%d|", t / 60 / 10, t / 60 % 10, t % 60 / 10, t % 60 % 10);
	return 0;
}

