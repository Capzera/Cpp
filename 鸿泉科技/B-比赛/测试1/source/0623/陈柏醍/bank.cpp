#include<bits/stdc++.h>
using namespace std;
int main()
{
	int m,xiangtongtianshu=0,n=0,xiangtongtianshuleijia = 0,qianshuleijia = 0;
	cin>>m;
	while(m!=0)
	{
		xiangtongtianshu+=1;
		xiangtongtianshuleijia += xiangtongtianshu;
		qianshuleijia += xiangtongtianshu*xiangtongtianshu;
		if(xiangtongtianshuleijia > m){
			int duochutianshu = xiangtongtianshuleijia - m;
			int duochuqianshu = duochutianshu * xiangtongtianshu;
			qianshuleijia -= duochuqianshu;
			break;
		}		
	}
	 
	cout<<qianshuleijia;
	return 0;
}
