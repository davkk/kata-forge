#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Map {
    void set(const string& k, int v);
    optional<int> get(const string& k);
    void remove(const string& k);
    int size();
};
