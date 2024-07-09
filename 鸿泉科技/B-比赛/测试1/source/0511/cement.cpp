#include<bits/stdc++.h>
using namespace std;
int main(){
	int n,a,k,b,m,c;
	cin >> n >> a >> k >> b;
	if(n%2 == 0){
		c=n/2;
	}else{
		cout << "-1";
		return 0;
	}
	if(k/b < a){
		if(n/2%k != 0){
			m=b*2*(n/2/k)+a*2*(n/2%k);
		}else{
			m=n/k*b;
		}
	}else{
		m=n*a;
	}
	cout << m;
	return 0;
}

