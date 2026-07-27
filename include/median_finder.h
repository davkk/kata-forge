#pragma once
#include <bits/stdc++.h>
using namespace std;

struct MedianFinder {
    priority_queue<int> lo;
    priority_queue<int, vector<int>, greater<>> hi;
    void insert(int val);
    double getMedian();
    int length();
};
