#include <bits/stdc++.h>
using namespace std;
int grid[5][5];
vector<vector<vector<vector<int> > > > dir = {
	{
		{{0, 0}, {0, 1}, {-1, 0}},
		{{0, 0}, {0, -1}, {-1, 0}},
		{{0, 0}, {0, 1}, {1, 0}},
		{{0, 0}, {0, -1}, {1, 0}}
	},
	{
		{{0, 0}, {0, 1}, {-1, 0}, {0, 2}, {-2, 0}},
		{{0, 0}, {0, -1}, {-1, 0}, {0, -2}, {-2, 0}},
		{{0, 0}, {0, 1}, {1, 0}, {0, 2}, {2, 0}},
		{{0, 0}, {0, -1}, {1, 0}, {0, -2}, {2, 0}}
	},
	{
		{{0, 0}, {0, 1}, {-1, 0}, {0, 2}, {-2, 0}, {0, 3}, {-3, 0}},
		{{0, 0}, {0, -1}, {-1, 0}, {0, -2}, {-2, 0}, {0, -3}, {-3, 0}},
		{{0, 0}, {0, 1}, {1, 0}, {0, 2}, {2, 0}, {0, 3}, {3, 0}},
		{{0, 0}, {0, -1}, {1, 0}, {0, -2}, {2, 0}, {0, -3}, {3, 0}}
	}
};
long long ans = 0;
void dfs(int num) {
	if (num == 3) {
		ans++;
		return;
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (grid[i][j] == 1) continue;
			for (int d = 0; d < 4; d++) {
				bool flag = 1;
				for (int l = 0; l < 2 * num + 3; l++) {
					int mx = i + dir[num][d][l][0], my = j + dir[num][d][l][1];
					if (mx < 0 || mx == 4 || my < 0 || my == 4 || grid[mx][my]) {
						flag = 0;
						break;
					}
				}
				if (flag) {
					for (int l = 0; l < 2 * num + 3; l++) {
						grid[i + dir[num][d][l][0]][j + dir[num][d][l][1]] = 1;
					}
					dfs(num + 1);
					for (int l = 0; l < 2 * num + 3; l++) {
						grid[i + dir[num][d][l][0]][j + dir[num][d][l][1]] = 0;
					}
				}
			}
		}
	}
}
int main() {
	grid[1][2] = 1;
	dfs(0);
	cout << ans << endl;
	return 0;
}
