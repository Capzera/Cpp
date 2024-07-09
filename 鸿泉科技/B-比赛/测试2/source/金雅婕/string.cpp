#include<iostream>
#include<cstring>
using namespace std;
string a,b;
int a1,b1,aa[100],bb[100],cc[100],aa2[100],bb2[100],cc2[100];
int main(){
	cin>>a>>b;
	for(int i=0;i<a.size();i++){
		if(a[i]=='1') a1++;
	}
	for(int i=0;i<b.size();i++){
		if(b[i]=='1') b1++;
	}
	//cout<<a1<<" "<<b1<<endl;
	for(int i=a.size()-a1+2;i<=a.size();i++){
		aa[i]=1;
		aa2[i]=1;
	}
	aa[1]=1;
	aa2[a.size()-a1+1]=1;
	//cout<<endl;
	for(int i=b.size()-b1+1;i<=b.size();i++){
		bb[i]=1;
		bb2[i]=1;
	}
	bb2[1]=1;
	bb2[b.size()-b1+1]=0;
	int len=max(a.size(),b.size());
	for(int i=1;i<=len;i++){
		cc[i]+=aa[i]+bb[i];
		cc2[i]+=aa2[i]+bb2[i];
		if(cc[i]>=2){
			cc[i+1]++;
			cc[i]-=2;
		}
		if(cc2[i]>=2){
			cc2[i+1]++;
			cc2[i]-=2;
		}
	}
	while(cc2[len+1] || cc[len+1]) len++;
	//cout<<len<<" ";
	for(int i=len;i>=1;i--){
		if(cc[i]>cc2[i]){
			for(int j=i;j>=1;j--) cout<<cc[j];
			return 0;
		}
		if(cc2[i]>cc[i]){
			for(int j=i;j>=1;j--) cout<<cc2[j];
			return 0;
		}
		cout<<cc[i];
	}
}
