/* Capzera 机长大大 */
#include<bits/stdc++.h>
#define MAXSIZE 100
using namespace std;
struct List
{
	int num[MAXSIZE+1];
	int length;
} L;
int InsList(List &L,int x)
{
	if(L.length == MAXSIZE)
	{
		cout<<"Error! The List is full."<<endl;
        L.num[0] = -1;
		return 0;
	}
	int *p,*q;
	p = L.num+L.length+1;
	q = L.num+2;
	for (; p>=q; p--)
	{
		if (*(p-1)>x)  *p = *(p-1);
		else break;
	}
	if (p == q) L.num[1] = x;
		else *p = x;
	L.length++;
	return 0;
}
void InputList(List &L)
{
	int n;
	cin>>n;
	L.length = n;
    L.num[0]=0;
	for(int i=1; i<=n; i++) cin>>L.num[i];
}
void PrintList(List L)
{
	for(int i=1; i<=L.length; i++) cout<<L.num[i]<<" ";
}
int main()
{
	int x;
	List L;
	InputList(L);
	cin>>x;
    InsList(L,x);
	if (L.num[0]==0) PrintList(L);
	system("pause");
	return 0;
}