#include <iostream> 
using namespace std;
int main() {
int n;
cin >> n;
if (n % 2 == 1 || n % 3 == 0) {
	cout << 1 << endl; 
}
else {
	cout << 0 << endl;
}
return 0;

}
