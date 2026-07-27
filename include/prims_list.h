#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Edge { int to; int weight; };

optional<vector<vector<Edge>>> prims(const vector<vector<Edge>>& g);
