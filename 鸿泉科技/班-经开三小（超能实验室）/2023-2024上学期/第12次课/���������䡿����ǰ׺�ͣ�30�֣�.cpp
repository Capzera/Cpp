#include<bits/stdc++.h>
using namespace std;
const int maxn = 1e6;
int a[maxn+5],s[maxn+5];
int n; 
long long ans;
int main(){
	cin >> n;
	m[0] = 1;
	for(int i = 1; i <= n; i++){
		cin >> a[i];
		if(a[i] % 2 != 0) s[i] = s[i-1] + 1;
		else s[i] = s[i-1] - 1;
		int cnt = 0;
		for(int j = 0; j < i; j++){
			if(s[j] < s[i]) cnt++;
		} 
		ans += cnt;
	}
	cout << ans; 
    return 0;
}
