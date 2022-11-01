#ifndef _COMMON_H_
#define _COMMON_H_

#include <graphics.h>
#include <fstream>
#include <bits/stdc++.h>
#include <windows.h>
#include <map>
#include <vector>
#include <set>

using namespace std;

typedef pair<int, int> pii;

struct pos {
	char x;
	int y;
	bool operator == (const pos& value) const{
		return (value.x == x && value.y == y);
	}
	bool operator < (const pos& value) const{
		if (x < value.x || y < value.y) return 1;
		else return 0;
	}
};

#endif //commom.h

