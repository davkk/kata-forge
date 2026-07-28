#include "kata_test.h"

int main() {
    { TEST("standard_window");
        vector<int> _arr = {1, 3, -1, -3, 5, 3, 6, 7};
        int _k = 3;
        auto _r = sliding_window_max(_arr, _k);
        { auto _exp = vector<int>{3, 3, 5, 5, 6, 7}; EQL(_exp, _r); }
    }

    { TEST("single_element");
        vector<int> _arr = {1};
        int _k = 1;
        auto _r = sliding_window_max(_arr, _k);
        { auto _exp = vector<int>{1}; EQL(_exp, _r); }
    }

    { TEST("two_elements");
        vector<int> _arr = {1, -1};
        int _k = 1;
        auto _r = sliding_window_max(_arr, _k);
        { auto _exp = vector<int>{1, -1}; EQL(_exp, _r); }
    }

    { TEST("increasing_input");
        vector<int> _arr = {9, 11};
        int _k = 2;
        auto _r = sliding_window_max(_arr, _k);
        { auto _exp = vector<int>{11}; EQL(_exp, _r); }
    }

    { TEST("decreasing_input");
        vector<int> _arr = {4, -2};
        int _k = 2;
        auto _r = sliding_window_max(_arr, _k);
        { auto _exp = vector<int>{4}; EQL(_exp, _r); }
    }

    return 0;
}
