#pragma once
#include <bits/stdc++.h>
using namespace std;

struct UnionFind {
    UnionFind(int n);
    int find(int x);
    bool unite(int x, int y);
    bool connected(int x, int y);
    int size();
};
