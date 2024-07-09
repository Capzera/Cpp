#include<bits/stdc++.h>
using namespace std;
const int maxn = 1e6;
int a[maxn+5],s[maxn+5];
int n; 
long long ans;
map<int,int> m; 
int main(){
	cin >> n;
	m[0] = 1;
	for(int i = 1; i <= n; i++){
		cin >> a[i];
		if(a[i] % 2 != 0) s[i] = s[i-1] + 1;
		else s[i] = s[i-1] - 1;
		for(auto it = m.begin(); it != m.end(); it++){
			if(it->first >= s[i]) break;
			ans += it->second;
		}
		m[s[i]]++;
	}
	cout << ans; 
    return 0;
}
