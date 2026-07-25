#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Node { int val; Node* left; Node* right; };

bool solve(Node* root, int64_t needle);
