#include <bits/stdc++.h>
using namespace std;
bool flag;
int add(int a, int b) {
	return a + b;
}

int sub(int a, int b) {
	return a - b;
}

int pro(int a, int b) {
	return a * b;
}

int chu(int a, int b) {
	//divisor can not be zero!!!!
	if (b == 0) {
		cout << "除数为 0"<< endl;
		flag = 1;
		return 0;
	} 
	else return a / b;
}

int pow1(int a, int b) {
	int ans = 1;
	for (long long i = 0; i < b; i++) {
		ans *= a;
	}
	return ans;
}

long long main() {
	int a, b, ans;
	char c;
	cin >> a >> c >> b;
	if (c == '+') {
		ans = add(a, b);
	} else if (c == '-') {
		ans = sub(a, b);
	} else if (c == '*') {
		ans = pro(a, b);
	} else if (c == '/') {
		ans = chu(a, b);
	} else if (c == '^') {
		ans = pow1(a, b);
	} else {
		cout <<"Error" <<endl;
	}
	if (!flag) {
		cout << ans << endl;
	}
	return 0;
}
