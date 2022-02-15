#include<iostream>
#include<stdio.h>
using namespace std;
int main()
{
	int v1,v2,t,s,l,i,RL=0,TL=0,rest=0,resume=0;
	cin>>v1>>v2>>t>>s>>l;
	int final=l/v2;
	for(i=1;i<=final;i++)
	{
		if (rest==1&&i>resume) rest=0;
		TL+=v2;
		if (rest==0) RL+=v1;
		if ((RL-TL)>=t)
		{
			rest=1;
			resume=i+s;
		}
		if (RL>=l&&TL<l)
		{
			cout<<"R"<<endl<<i;
			return 0;
		}
	}
	if (RL==TL)cout<<"D"<<endl<<final;
	else cout<<"T"<<endl<<final;
}
