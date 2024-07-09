#include<iostream>
#include<cmath>
using namespace std;
int main()
{
	int a,o;
	cin>>a>>o;
	int ty[a+1],td[o+1];
	int b,m=1,t=5,d=0;
	for(int i=1;i<=a;i++)
	{
		cin>>ty[i];
	}
	for(int i=1;i<=o;i++)
	{
		cin>>td[i];
	}
	for(int w=0;w<o;w++)
	{
	for(int i=1;i<=a;i++)
	{
		b=ty[i];
		m=max(b,m);
	}
	for(int i=1;i<=o;i++)
	{
		b=td[i];
		t=min(b,t);
	}
	if(m>t) d++;
	for(int i=1;i<=a;i++)
	{
		if(m==ty[i]) 
		{
		ty[i]=0;
		i=a;
		}
	}
	for(int i=1;i<=o;i++)
	{
		if(t==td[i]) 
		{
		td[i]=999;
		i=o;
		}
	}
	}
	cout<<d;
	return 0;
}
