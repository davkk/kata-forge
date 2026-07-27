#include "kata_test.h"

int main() {
    { TEST("partial_selection");
        int _n = 4;
        int _k = 2;
        auto _r = combine(_n, _k);
        vector<vector<int>> _exp_r = {{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}};
        sort(_exp_r.begin(), _exp_r.end());
        sort(_r.begin(), _r.end());
        EQL(_exp_r, _r);
    }

    { TEST("full_selection");
        int _n = 4;
        int _k = 4;
        auto _r = combine(_n, _k);
        vector<vector<int>> _exp_r = {{1, 2, 3, 4}};
        sort(_exp_r.begin(), _exp_r.end());
        sort(_r.begin(), _r.end());
        EQL(_exp_r, _r);
    }

    { TEST("empty_selection");
        int _n = 4;
        int _k = 0;
        auto _r = combine(_n, _k);
        vector<vector<int>> _exp_r = {{}};
        sort(_exp_r.begin(), _exp_r.end());
        sort(_r.begin(), _r.end());
        EQL(_exp_r, _r);
    }

    return 0;
}
