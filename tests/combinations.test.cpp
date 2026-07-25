#include <cassert>

int main() {
    { // n4k2
        int64_t _n = 4;
        int64_t _k = 2;
        auto _r = combine(_n, _k);
        auto _exp_r = {{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}};
        sort(_exp_r.begin(), _exp_r.end());
        sort(_r.begin(), _r.end());
        assert(_r == _exp_r);
    }

    { // n4k4
        int64_t _n = 4;
        int64_t _k = 4;
        auto _r = combine(_n, _k);
        auto _exp_r = {{1, 2, 3, 4}};
        sort(_exp_r.begin(), _exp_r.end());
        sort(_r.begin(), _r.end());
        assert(_r == _exp_r);
    }

    { // n4k0
        int64_t _n = 4;
        int64_t _k = 0;
        auto _r = combine(_n, _k);
        auto _exp_r = {{}};
        sort(_exp_r.begin(), _exp_r.end());
        sort(_r.begin(), _r.end());
        assert(_r == _exp_r);
    }

    return 0;
}
