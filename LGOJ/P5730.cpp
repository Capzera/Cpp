#include<bits/stdc++.h>
using namespace std;
char W[10][5][3] = {
	{
		'X', 'X', 'X',
		'X', '.', 'X',
		'X', '.', 'X',
		'X', '.', 'X',
		'X', 'X', 'X',},
	{
		'.', '.', 'X',
		'.', '.', 'X',
		'.', '.', 'X',
		'.', '.', 'X',
		'.', '.', 'X',},
	{
		'X', 'X', 'X',
		'.', '.', 'X',
		'X', 'X', 'X',
		'X', '.', '.',
		'X', 'X', 'X',},
	{
		'X', 'X', 'X',
		'.', '.', 'X',
		'X', 'X', 'X',
		'.', '.', 'X',
		'X', 'X', 'X',},
	{
		'X', '.', 'X',
		'X', '.', 'X',
		'X', 'X', 'X',
		'.', '.', 'X',
		'.', '.', 'X',},
	{
		'X', 'X', 'X',
		'X', '.', '.',
		'X', 'X', 'X',
		'.', '.', 'X',
		'X', 'X', 'X',},
	{
		'X', 'X', 'X',
		'X', '.', '.',
		'X', 'X', 'X',
		'X', '.', 'X',
		'X', 'X', 'X',},
	{
		'X', 'X', 'X',
		'.', '.', 'X',
		'.', '.', 'X',
		'.', '.', 'X',
		'.', '.', 'X',},
	{
		'X', 'X', 'X',
		'X', '.', 'X',
		'X', 'X', 'X',
		'X', '.', 'X',
		'X', 'X', 'X',},
	{
		'X', 'X', 'X',
		'X', '.', 'X',
		'X', 'X', 'X',
		'.', '.', 'X',
		'X', 'X', 'X',}
};
int n;
string s;
int main() {
	cin >> n >> s;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < 3; k++) cout << W[s[j] - '0'][i][k];
			if (j != n - 1) cout << '.';
		}
		cout << endl;
	}
	return 0;
}