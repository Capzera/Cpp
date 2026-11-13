#include<bits/stdc++.h>
#include<iomanip>
using namespace std;
int main(){
	int n,m;
	double x,zzz;
	cin>>m>>x;
    zzz=x;
	double min3=10000000000000;
	int min1,min2;
	bool g=false;
	for(int i=1;i<=m;i++){
		double u=i*x;
		u+=0.5;
		int y=u;
		if(abs(double(y)/double(i)-x)<=min3&&__gcd(y,i)==1){
			if(min3==abs(double(y)/double(i)-x)){
				g=true;
			}else{
				min1=y,min2=i,min3=abs(double(y)/double(i)-x);
				g=false;
			}	
		}
		if(abs(double(n)/double(i)-x)>min3)continue;
		if(__gcd(n,i)!=1)continue;
		if(min3==abs(double(n)/double(i)-x)){
			g=true;
		}else{
			min1=n,min2=i,min3=abs(double(n)/double(i)-x);
			g=false;
		}
	}
	if(g)cout<<"More than one!"<<endl;
	else {
        cout<<min1;
        if (min2!=1) cout<<"/"<<min2;
        double tmp=double(min1)/double(min2)-zzz;
        cout<<" "<<setiosflags(ios::fixed)<<setprecision(10)<<abs(tmp);
    }
	return 0;
}
/*
120 3.1415926536
*/