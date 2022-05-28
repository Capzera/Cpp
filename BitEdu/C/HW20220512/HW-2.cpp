#include <bits/stdc++.h>
using namespace std;
int main()
{
	int m;
	cin>>m;
	for (int i=31;i>=1;i-=2) cout<<((m>>i)&1);
	cout<<endl;
	for (int i=30;i>=0;i-=2)cout<<((m>>i)&1);
    system("pause");
	return 0;
}