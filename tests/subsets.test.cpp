#include "kata_test.h"

int main() {
    { TEST("three_elements");
        vector<int> _arr = {1, 2, 3};
        auto _r = subsets(_arr);
        vector<vector<int>> _exp_r = {{}, {1}, {2}, {3}, {1, 2}, {1, 3}, {2, 3}, {1, 2, 3}};
        sort(_exp_r.begin(), _exp_r.end());
        sort(_r.begin(), _r.end());
        EQL(_exp_r, _r);
    }

    { TEST("empty");
        vector<int> _arr = {};
        auto _r = subsets(_arr);
        vector<vector<int>> _exp_r = {{}};
        sort(_exp_r.begin(), _exp_r.end());
        sort(_r.begin(), _r.end());
        EQL(_exp_r, _r);
    }

    { TEST("single_element");
        vector<int> _arr = {1};
        auto _r = subsets(_arr);
        vector<vector<int>> _exp_r = {{}, {1}};
        sort(_exp_r.begin(), _exp_r.end());
        sort(_r.begin(), _r.end());
        EQL(_exp_r, _r);
    }

    return 0;
}
