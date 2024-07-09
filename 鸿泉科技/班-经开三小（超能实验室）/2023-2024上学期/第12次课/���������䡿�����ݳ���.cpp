#include<bits/stdc++.h>
using namespace std;
const int off = 1e6+1;
const int maxn = 1e6;
int a[maxn+5],s[maxn+5],c[2*maxn+5];
int n; 
long long ans;
int lowbit(int x){
	return x&-x;
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
	while(x <= 2*off){
		c[x] += k;
		x += lowbit(x);
	}
}
int main(){
	ofstream file("1--10.in");
	ofstream out("1--10.out");
	int n = 1e6;
	file << n << endl;
	add(off,1);
	for(int i = 1; i <= n; i++){ 
		a[i] = rand()%int(1e8)-10000;
	//ÌØA 
//		a[i] *= 2;
	
	//ÌØB
//    	a[i] = a[i]*2+1;
	
	//ÌØC 
//    	if(s[i-1] == 10) a[i] += (a[i]%2);
//    	if(s[i-1] == -10) a[i] += (a[i]%2 == 0); 
	
		file << a[i] << " ";
		if(a[i] % 2 != 0) s[i] = s[i-1] + 1;
		else s[i] = s[i-1] - 1;
		ans += getSum(s[i]+off-1);
		add(s[i]+off,1);
	}
	out << ans; 
    return 0;
}

