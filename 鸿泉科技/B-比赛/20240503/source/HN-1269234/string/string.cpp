#include<bits/stdc++.h>

using namespace std;

string s;
int a = 0;

int main(){
	freopen("string.in","r",stdin);
	freopen("string.out","w",stdout);
	cin >> s;
	
	
	
		for(int j = 0; j < s.size() - 1; j++){
			if(s[i] != s[i + 1]){
				a++;
			}
		}	
	
	cout << a;
	return 0;
}

