#pragma once
#include <bits/stdc++.h>
using namespace std;

struct HashSet {
    void add(int x);
    void remove(int x);
    bool has(int x);
    int size();
};
