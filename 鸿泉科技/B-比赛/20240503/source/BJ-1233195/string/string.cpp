#include <bits/stdc++.h>

using namespace std;
string s[10000010];
int main(){
	freopen("string.in","r",stdin);
	freopen("string.out","w",stdout);
	int n=0;
	int ans=0;
	cin>>n;
    for(int i=0;i<=n;i++){
		cin>>s[i];
	}
	if(n==1){
		cout<<"1"<<endl;
		return 0;
	}
	for(int i=0;i<n;i++){
		ans+=n;
		if(s[i]!=s[i+1]){
			ans++;
		}
	}
	cout<<ans<<endl;
	return 0;
}
