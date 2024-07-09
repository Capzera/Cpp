#include<bits/stdc++.h>
using namespace std;
int main(){
	int m,sum=0;
	cin>>m;
	for(int i=1;i<m;i++)
	{
		for(int j=1;j<m;j++)
		{
			sum=i*j/2;
		}
	}
	cout<<sum;
	return 0;
}

