#pragma once
#include <bits/stdc++.h>
using namespace std;

struct LRU {
    int cap;
    void update(const string& k, int v);
    optional<int> get(const string& k);
    int size();
};
