#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Edge { int to; int weight; };

optional<vector<int>> bfs(const vector<vector<Edge>>& g, int source, int sink);
