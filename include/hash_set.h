#pragma once
#include <bits/stdc++.h>
using namespace std;

struct HashSet {
    vector<vector<int>> buckets;
    void add(int x);
    void remove(int x);
    bool contains(int x);
    int length();
};
