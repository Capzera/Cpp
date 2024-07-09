#include<bits/stdc++.h>
using namespace std;
long long n, k;
long long a[1000005], sum[1000005];
int main() {
    cin >> n >> k;
    for (int i = 0; i < n; i++) {
    	cin >> a[i];
        a[i + n] = a[i];
    	sum[i] = a[i] + sum[i - 1];
    	
	}
	for (int i = n; i < 2 * n; i++) {
		sum[i] = a[i] + sum[i- 1];
	} 
	for (int i = 0; i < n; i++){
    	if (k > 0) {
    	   cout << sum[i + k] - sum[i] << " ";	
		}
		if (k < 0) {
    	   cout << sum[i + n - 1] - sum[i + n + k - 1] << " ";
		}
	}
	return 0;
}
