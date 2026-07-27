#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Map {
    struct KV { string key; int val; };
    vector<vector<KV>> buckets;
    void set(const string& k, int v);
    optional<int> get(const string& k);
    void erase(const string& k);
    int size();
};
