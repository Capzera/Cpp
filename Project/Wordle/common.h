#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <tuple>
#include <graphics.h>
#include <conio.h>
#include <mmsystem.h>
#include <windows.h>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;
static HWND hd = GetDesktopWindow();
static int SCREENWIDTH=GetSystemMetrics(SM_CXSCREEN);
static int SCREENHEIGHT=GetSystemMetrics(SM_CYSCREEN);
static double ZOOM=GetDpiForWindow(hd)/96;

#endif
