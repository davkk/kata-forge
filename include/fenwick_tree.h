#pragma once
#include <bits/stdc++.h>
using namespace std;

struct FenwickTree {
    int n;
    void add(int idx, int delta);
    int sum(int idx);
    int size();
};
