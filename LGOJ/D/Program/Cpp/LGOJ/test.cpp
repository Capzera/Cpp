#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1e5+10;
ll m,k,p[N];
ll phi(ll x){
	ll ans=x;
	for(int i=2;i*i<=x;i++){
		if(x%i==0){
			ans-=ans/i;
			while(x%i==0){
				x/=i;
			}
		}
	}
	if(x>1) ans-=ans/x;
	return ans;
} //求欧拉函数。
int main(){
	//freopen(".in","r",stdin);
	//freopen(".out","w",stdout);
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	int t;
	cin>>t;
	while(t--){
		cin>>m>>k;
		for(int i=1;i<=k;i++){
			cin>>p[i];
		}
		if(k==9){
			cout<<"-1\n";
			continue;
		}
		int num=0;
		if(m%2==0||m%5==0){
			int num2,num5;
			num2=num5=0;
			while(m%2==0){
				m/=2;
				num2++;
			}
			while(m%5==0){
				m/=5;
				num5++;
			}
			num=max(num2,num5);
		} // 2，5 先处理。
		set<int> s;
		for(int i=0;i<=9;i++){
			int flag=0;
			for(int j=1;j<=k;j++){
				if(i==p[j]) flag=1;
			}
			if(flag==1) continue;
			else if(flag==0) s.insert(i);
		}
		if(num!=0) cout<<"2\n";
		else cout<<"1\n";
		if(m==1) cout<<*s.rbegin()<<" 114514\n";
		else{
			cout<<*s.rbegin()<<" "<<phi(9*m)<<"\n";
		}
		if(num!=0) cout<<"0 "<<num<<"\n";
	}  //注意输出格式。
	return 0;
}

