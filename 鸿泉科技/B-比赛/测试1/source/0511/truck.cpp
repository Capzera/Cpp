#include<bits/stdc++.h>
using namespace std;
int main(){
	int a,b,x,k,n;
	cin >> a >> b >> x >> k;
	for(int i=1;i<=a;i++){
		if(i%k == 0&&b>0){
			a=a+1;
		}else{
			n=i*x;
		}
	}
	cout << n;
	return 0;
}

