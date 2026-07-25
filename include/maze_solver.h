#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Point { int x, y; };

vector<Point> solve(const vector<string>& maze, const string& wall, Point start, Point end);
