#include<iostream>
using namespace std;

int main()
{
 	int a,b,x,k;
 	cin>>a>>b>>x>>k;
 	int w=0;
 	w=a*x;
 	int y=a/k;
	if(k>y) k=y;
	w=w+k*x; 
	cout<<w;
	return 0;
}
