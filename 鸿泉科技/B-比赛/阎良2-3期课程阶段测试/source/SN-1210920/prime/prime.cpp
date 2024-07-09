#include<bits/stdc++.h>
using namespace std;
long long n,o=10000,s=0,e=100,g=0;
int main()
{
	freopen("prime.in","r",stdin);
	freopen("prime.out","w",stdout);
	cin>>n;
	long long a[n]; 
	for(int i=0;i<n;i++)
	{
		a[i]=0; 
	    cin>>a[i];
	}
	for(int i=0;i<n;i++)
	{
		int j=0;
		while(o!=0)
		{
		    s=a[i]-j;
		    if(s==1)
		    {
		    	s++;
				o=0; 
			}
		    if(s==2||s==3||s==5||s==7)
		    {
				o=0;
			}
			else if(s%2!=0&&s%3!=0&&s%5!=0&&s%7!=0)
			{
				o=0;
			}
			j++;
		}
		j=0;
		while(e!=0)
		{
		    g=a[i]+j;
		    if(g==1)
		    {
		    	g++;
				e=0; 
			}
		    if(g==2||g==3||g==5||g==7)
		    {
				e=0;
			}
			else if(g%2!=0&&g%3!=0&&g%5!=0&&g%7!=0)
			{
				e=0;
			}
			j++;
		}
		if(a[i]-s==g-a[i])
		{
			cout<<s<<" ";
		}
		else if(a[i]-s>g-a[i])
		{
			cout<<g<<" ";
		}
		else 
		{
			cout<<s<<" ";
		}
		o=100;
		e=100;
	}
	return 0;
}
