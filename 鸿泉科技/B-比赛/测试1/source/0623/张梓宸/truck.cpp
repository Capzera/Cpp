#include<bits/stdc++.h>
using namespace std;
int main()
{
	int a,b,x,k,s,w=1;
	cin>>a>>b>>x>>k;
	while(a!=0)
	{
		w++;
		if(w>k) w=0;
		a-=1;
		if(w==k)
		{
			a++;
			b--;
		} 
		s+=x;
		if(a==0) break;
		cout<<a<<" "<<b<<" "<<s<<" "<<w<<" "<<endl;
	}
	cout<<s;
	return 0;
} 
