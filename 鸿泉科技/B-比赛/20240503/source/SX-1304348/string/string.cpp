#include<bits/stdc++.h>
using namespace std;
string s;
long long sum=0;
int main(){
	freopen("string.in","r",stdin);
	freopen("string.out","w",stdout);
	cin>>s;
	sum+=s.size();
	for(int i=0;i<s.size()-1;i++){
		if(s[i]!=s[i+1]){
			sum++;
		}
	}
	cout<<sum;
	return 0;
}

