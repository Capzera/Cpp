#include<bits/stdc++.h>
using namespace std;
int main() {
	int n,m,sum=0;
	scanf("%d %d",&n,&m);
	for (int i=n-m+1;i<=n;i++) sum+=i;
	printf("%d\n",sum);
	system("pause");	
	return 0;
}