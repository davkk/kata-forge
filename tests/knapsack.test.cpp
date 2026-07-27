#include "kata_test.h"

int main() {
    { TEST("mixed_weights");
        int _cap = 10;
        vector<int> _w = {5, 4, 6, 3};
        vector<int> _v = {10, 40, 30, 50};
        auto _r = knapsack(_cap, _w, _v);
        CHECK(_r == 90);
    }

    { TEST("small_set");
        int _cap = 5;
        vector<int> _w = {2, 3, 4};
        vector<int> _v = {3, 4, 5};
        auto _r = knapsack(_cap, _w, _v);
        CHECK(_r == 7);
    }

    { TEST("zero_capacity");
        int _cap = 0;
        vector<int> _w = {1, 2};
        vector<int> _v = {5, 6};
        auto _r = knapsack(_cap, _w, _v);
        CHECK(_r == 0);
    }

    { TEST("standard_items");
        int _cap = 50;
        vector<int> _w = {10, 20, 30};
        vector<int> _v = {60, 100, 120};
        auto _r = knapsack(_cap, _w, _v);
        CHECK(_r == 220);
    }

    return 0;
}
