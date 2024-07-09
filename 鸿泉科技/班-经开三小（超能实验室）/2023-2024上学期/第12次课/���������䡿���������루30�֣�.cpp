#include<bits/stdc++.h>
using namespace std;
const int maxn = 1e6;
int a[maxn+5];
int n; 
long long ans;
int main(){
	cin >> n;
	for(int i = 1; i <= n; i++){
		cin >> a[i];
	}
	for(int i = 1; i <= n; i++){
		for(int j = i; j <= n; j++){
			int odd = 0,even = 0;
			for(int k = i; k <= j; k++){
				if(a[k] % 2 == 0) even++;
				else odd++;
			}
			if(odd > even) ans++;
		}
	}
	cout << ans;
    return 0;
}

