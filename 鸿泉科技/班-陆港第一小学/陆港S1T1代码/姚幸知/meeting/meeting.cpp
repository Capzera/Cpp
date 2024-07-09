#include <iostream> 
using namespace std;
int main (){
	int t,m,s,v;
	cin>>t>>m>>s>>v;
	int e=s/v+1;
	int a=e+m;
	int b=t*60;
	int c=b-a;
	int y=c%60;
	int d=c-y;
	int x=d/60; 
	cout<<x<<" "<<y;
	return 0;
}
