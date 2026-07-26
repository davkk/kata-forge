#include "kata_test.h"

int main() {
    { TEST("standard_k3");
        vector<int> _arr = {1, 3, -1, -3, 5, 3, 6, 7};
        int64_t _k = 3;
        auto _r = solve(_arr, _k);
        { auto _exp = vector<int>{3, 3, 5, 5, 6, 7}; CHECK(_r == _exp); }
    }

    { TEST("single_element");
        vector<int> _arr = {1};
        int64_t _k = 1;
        auto _r = solve(_arr, _k);
        { auto _exp = vector<int>{1}; CHECK(_r == _exp); }
    }

    { TEST("two_elements_k1");
        vector<int> _arr = {1, -1};
        int64_t _k = 1;
        auto _r = solve(_arr, _k);
        { auto _exp = vector<int>{1, -1}; CHECK(_r == _exp); }
    }

    { TEST("increasing_k2");
        vector<int> _arr = {9, 11};
        int64_t _k = 2;
        auto _r = solve(_arr, _k);
        { auto _exp = vector<int>{11}; CHECK(_r == _exp); }
    }

    { TEST("decreasing_k2");
        vector<int> _arr = {4, -2};
        int64_t _k = 2;
        auto _r = solve(_arr, _k);
        { auto _exp = vector<int>{4}; CHECK(_r == _exp); }
    }

    return 0;
}
