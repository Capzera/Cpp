#include<bits/stdc++.h>
using namespace std;
int main(){
    int t,n,a,b;
	string Y="YES",N="NO";
	cin>>t;
	for (int T=0;T<t;T++) {
		cin>>n;
		priority_queue<int> q;
		int put;
		for (int i=0;i<n;i++) {
			cin>>put;
			q.push(put);
		}
		a=q.top();
		q.pop();
		if (q.size()) b=q.top();
		else b=0;
		if(a-b>1) cout<<N<<endl;
		else cout<<Y<<endl;
	}
	system("pause");
    return 0;
}