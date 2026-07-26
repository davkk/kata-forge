#include "kata_test.h"

int main() {
    { TEST("three");
        vector<int64_t> _arr = {1, 2, 3};
        auto _r = subsets(_arr);
        auto _exp_r = {{1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1}};
        sort(_exp_r.begin(), _exp_r.end());
        sort(_r.begin(), _r.end());
        CHECK(_r == _exp_r);
    }

    { TEST("empty");
        vector<int64_t> _arr = {};
        auto _r = subsets(_arr);
        auto _exp_r = {{}};
        sort(_exp_r.begin(), _exp_r.end());
        sort(_r.begin(), _r.end());
        CHECK(_r == _exp_r);
    }

    { TEST("one");
        vector<int64_t> _arr = {1};
        auto _r = subsets(_arr);
        auto _exp_r = {{1}};
        sort(_exp_r.begin(), _exp_r.end());
        sort(_r.begin(), _r.end());
        CHECK(_r == _exp_r);
    }

    return 0;
}
