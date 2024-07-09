#include<bits/stdc++.h>
using namespace std;
#define ll long long
ll m,ans,day;
int main(){
	cin>>m;
	for(ll i=1;i<=m;i++){
		for(ll j=1;j<=i;j++){
			ans=ans+i;
			day++;
			if(day==m){
				cout<<ans;
				return 0;
			}
		}
	}
	return 0;
} 
