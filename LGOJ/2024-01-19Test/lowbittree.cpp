#include<bits/stdc++.h>
using namespace std;
int n;
vector<int> t;
int lowbit(int x){
	return x & -x;
}
long long get(int x){
	long long ans = 0;
	for (; x; x -= lowbit(x)) {
		ans += t[x];
	}
	return ans;
}
void add(int x){
	for(; x <= 2 * n; x += lowbit(x)){
		t[x]++;
	}
}
int main(){
    long long ans = 0, sum = 0, x;
	cin >> n;
	t.resize(4 * n);
	add(n + 1);
	for(int i = 0; i < n; i++) {
		cin >> x;
		sum += x & 1 ? 1 : -1;
		ans += get(sum + n);
		add(sum + n + 1);
	}
	cout << ans << endl; 
    return 0;
}