#include <iostream>
 using namespace std;
 int main (){
 	int t,m,s,v,min,a,x,y,b;
 	cin >>t>>m>>s>>v;
 	min=t*60;
 	a=min-m-(s/v-s%v+2);
 	b=a/60-a%60;
 	y=a%60;
 	x=a/60;
    cout <<x<<" "<<y;
 	return 0;
 }
