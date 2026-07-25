#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Map {
    void set(const string& k, int64_t v);
    optional<int64_t> get(const string& k);
    void remove(const string& k);
    int size();
};
