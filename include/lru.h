#pragma once
#include <bits/stdc++.h>
using namespace std;

struct LRU {
    void update(const string& k, int64_t v);
    optional<int64_t> get(const string& k);
    int size();
};
