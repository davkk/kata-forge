#pragma once
#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    SegmentTree(vector<int> a);
    void update(int idx, int val);
    int query(int left, int right);
    int length();
};
