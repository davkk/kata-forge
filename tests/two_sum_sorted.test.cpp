#include "kata_test.h"

int main() {
    { TEST("indices_1_2");
        vector<int> _arr = {2, 7, 11, 15};
        int _k = 9;
        auto _r = two_sum(_arr, _k);
        { auto _exp = vector<int>{1, 2}; CHECK(_r == _exp); }
    }

    { TEST("indices_1_3");
        vector<int> _arr = {2, 3, 4};
        int _k = 6;
        auto _r = two_sum(_arr, _k);
        { auto _exp = vector<int>{1, 3}; CHECK(_r == _exp); }
    }

    { TEST("negative_target");
        vector<int> _arr = {-1, 0};
        int _k = -1;
        auto _r = two_sum(_arr, _k);
        { auto _exp = vector<int>{1, 2}; CHECK(_r == _exp); }
    }

    { TEST("indices_4_5");
        vector<int> _arr = {1, 2, 3, 4, 5};
        int _k = 9;
        auto _r = two_sum(_arr, _k);
        { auto _exp = vector<int>{4, 5}; CHECK(_r == _exp); }
    }

    return 0;
}
