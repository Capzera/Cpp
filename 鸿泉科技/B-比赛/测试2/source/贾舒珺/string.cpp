#include<iostream>
#include<algorithm>
using namespace std;
const int N=11;
int a1[N],a2[N],ans[N];
int main() {
	string s1,s2;
	cin>>s1>>s2;
	int s11=0,s21=0;
	for(int i=0; i<s1.size(); i++) {
		if(s1[i]=='1') s11++;
	}
	for(int i=0; i<s1.size(); i++) {
		int q=s1.size()-i;
		if(q<=s11) a1[i]=1;
		else a1[i]=0;
	}
	for(int i=0; i<s2.size(); i++) {
		if(s2[i]=='1') s21++;
	}
	for(int i=0; i<s2.size(); i++) {
		int q=s2.size()-i;
		if(q<=s21) a2[i]=1;
		else a2[i]=0;
	}
	int s10=s1.size()-s11,s20=s2.size()-s21;
	if(a1[0]+a2[0]==0||a1[0]+a2[0]==2) {
		if(s10==0) a2[s20-1]=1,a2[0]=0;
		else if(s20==0) a1[s10-1]=1,a1[0]=0;
		else if(s10<s20) a1[s10]=0,a1[0]=1;
		else a2[s20]=0,a2[0]=1;
	}
	int len=max(s1.size(),s2.size());
	for(int i=0; i<len; i++) {	
		if(ans[i]+a1[i]+a2[i]<2) ans[i]=ans[i]+a1[i]+a2[i];
		else {
			ans[i]=ans[i]+a1[i]+a2[i]-2;
			ans[i+1]++;
		}
	}
	if(ans[len]>0) cout<<ans[len];
	for(int i=len-1; i>=0; i--) {
		cout<<ans[i];
	}
	return 0;
}
