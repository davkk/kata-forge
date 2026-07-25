#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Edge { int to; int64_t weight; };

optional<vector<vector<Edge>>> prim(const vector<vector<Edge>>& g);
