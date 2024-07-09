#include<bits/stdc++.h>
using namespace std;
long long sum,sum1,a1,b1,m;
string a,b;
vector<int> v;
int main(){
	freopen("sum.in","r",stdin);
	freopen("sum.out","w",stdout);
	cin>>a;
	cin>>b;
	sort(a.begin(),a.end());
	sort(b.begin(),b.end());
	for(int i=a.size();i>=1;i--){
		sum+=(a[i-1]-'0')*pow(2,i-1);
		a1+=(a[i-1]-'0')*pow(2,i-1);
	}
	for(int i=b.size();i>=1;i--){
		sum+=(b[i-1]-'0')*pow(2,i-1);
		b1+=(a[i-1]-'0')*pow(2,i-1);
	}
	while(sum>0){
		m=sum%2;
		sum/=2;
		v.push_back(m);
	}
	for(int i=v.size()-1;i>=0;i--){
		cout<<v[i];
	}
}
