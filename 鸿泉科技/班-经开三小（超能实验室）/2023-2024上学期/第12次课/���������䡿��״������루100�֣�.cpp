#include<bits/stdc++.h>
using namespace std;
const int off = 1e6 + 1;
const int maxn = 1e6;
int a[maxn + 5], s[maxn + 5], c[2 * maxn + 5];
int n; 
long long ans;
int lowbit(int x){
	return x & -x;
}
long long getSum(int x){
	long long res = 0;
	while(x > 0){
		res += c[x];
		x -= lowbit(x);
	}
	return res;
}
void add(int x,int k){
	while(x <= 2 * off){
		c[x] += k;
		x += lowbit(x);
	}
}
int main(){
	cin >> n;
	add(off, 1);
	for(int i = 1; i <= n; i++){
		cin >> a[i];
		s[i] = s[i - 1] + a[i] % 2 != 0 ? 1 : -1;
		ans += getSum(s[i] + off - 1);
		add(s[i] + off,1);
	}
	cout << ans << endl; 
    return 0;
}

