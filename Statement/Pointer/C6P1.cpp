#include<bits/stdc++.h>
using namespace std;
void sort(int *,int *,int *);
int main(){
	int *a,*b,*c,a1,b1,c1;
	cout<<"Press input 3 numbers:"<<endl;
	cin>>a1>>b1>>c1;
    a=&a1;b=&b1;c=&c1;
	sort(a,b,c);
    cout<<"a="<<*a<<" b="<<*b<<" c="<<*c<<endl;
	system("pause");
	return 0;
}
void sort(int *a,int *b,int *c){
    void change(int *,int *);
	if (*a>*b) change(a,b);
	if (*a>*c) change(a,c);
	if (*b>*c) change(b,c);
}
void change(int *x,int *y){
	int temp;
	temp=*x;
	*x=*y;
	*y=temp;
}