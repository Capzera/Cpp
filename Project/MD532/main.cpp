#include "MD5.h"
#include <bits/stdc++.h>

using namespace std;

int main(){
	MD5 md5;
	string s;
	cout<<"请输入待加密的字符串:";
	cin>>s;
	cout<<"加密结果为:"<<md5.encode(s)<<endl;
	return 0;
}
/*
	1p6s4s8s1s1s8p2p7z9p4z2z9s6z1m2z8p5z6p8s2s5s4s0p5p6z8p9s7s1s9p3p9p4z1z2s5z7p3p1p5z1z9p7s4z1m8s5p2p1p4p6s1z1m3z4p5z1s3z3s1p9m6z3p9m3z5s3s
*/
