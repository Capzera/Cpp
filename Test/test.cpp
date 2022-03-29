#include<bits/stdc++.h>
using namespace std;
int main() {
	int n;
	cin>>n;
	vector<int> list(n);
	for (int i=0;i<n;i++) cin>>list[i];
	srand(time(NULL));
	int a=rand()%n;
	cout<<"Ëæ»úÊýÎª£º"<<a<<endl;
	cout<<list[a]<<endl;
	system("pause");
	return 0;
}