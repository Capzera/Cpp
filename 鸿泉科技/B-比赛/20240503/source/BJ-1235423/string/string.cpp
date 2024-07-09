#include <bits/stdc++.h>
using namespace std;

string s;
int main()
{
	freopen("string.in","r",stdin);
	freopen("string.out","w",stdout);
	int num,i,j,size;
	
	cin>>s;
	num=0;
	size=s.size();
	s.push_back(s[size-1]);
	for(i=0;i<size;i++)
	{
		num++;
		//cout<<num<<endl;
		for(j=i;j<size;)
		{
			if(s[j]!=s[j+1])
			{
				num++;
				j++;
			}
			else
			{
				break;
			}
		}
	}
	cout<<num;
	return 0;
}
