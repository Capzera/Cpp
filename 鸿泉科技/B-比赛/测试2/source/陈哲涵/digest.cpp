#include<iostream>
#include<cmath>
using namespace std;
int main()
{
	int a,m=0;
	cin>>a;
	int ty[a+1];
	int b,o,n=0,d=1;
	for(int i=1;i<=a;i++)
	{
		cin>>ty[i];
	}
	for(d=1;d<999;d++)
	{
	
	for(int i=1;i<=a;i++)
	{
		b=ty[i];
		m=min(b,m);
	}
	for(int i=1;i<=a;i++)
	{
		o=abs(ty[i]);
		n=max(o,n);
	}
	if(n==abs(m)) {
	cout<<d;
	break;
	}
	for(int i=1;i<=a;i++)
	{
		if(ty[i]==m) 
		{	
		ty[i]=999999999999;
		}
	}
	for(int i=1;i<=a;i++)
	{
		ty[i]=ty[i]-m;
	}
	}
	return 0;
}
