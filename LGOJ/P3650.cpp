#include <bits/stdc++.h>
using namespace std;
const int N = 1010;
int n, ans = INT_MAX, a[N];
int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	
	return 0;
}


给你一个长度为n的数组，每个数的大小在0-100中间
你可以修改任何一个数，x-y需要花费(x-y)^2，你要保证修改完的数组
最大值-最小值<= 17
