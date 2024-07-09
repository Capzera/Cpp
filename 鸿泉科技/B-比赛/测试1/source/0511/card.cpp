#include<bits/stdc++.h>
using namespace std;
int main(){
	int n,m,k;
	cin >> n >> m;
	int A[n],B[m];
	for(int i=0;i<n;i++){
		cin >> A[i];
	}
	for(int i=0;i<m;i++){
		cin >> B[i];
	}
	for(int i=0;i<n;i++){
		for(int j=0;j<m;j++){
			if(A[i]>B[j]){
				k=k+1;
				i++;
			}
		}
	}
	cout << k;
	return 0;
}

