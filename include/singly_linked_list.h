#pragma once
#include <bits/stdc++.h>
using namespace std;

struct SinglyLinkedList {
    void append(int x);
    void prepend(int x);
    void insertAt(int i, int x);
    optional<int> removeAt(int i);
    optional<int> remove(int x);
    optional<int> get(int i);
    int size();
};
