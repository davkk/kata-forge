#include <cassert>

int main() {
    { // a
        vector<int> _arr = {3, 2, 1, 5, 6, 4};
        int64_t _k = 2;
        auto _r = solve(_arr, _k);
        assert(_r == 5);
    }

    { // b
        vector<int> _arr = {3, 2, 3, 1, 2, 4, 5, 5, 6};
        int64_t _k = 4;
        auto _r = solve(_arr, _k);
        assert(_r == 4);
    }

    { // c
        vector<int> _arr = {1};
        int64_t _k = 1;
        auto _r = solve(_arr, _k);
        assert(_r == 1);
    }

    { // d
        vector<int> _arr = {2, 1};
        int64_t _k = 1;
        auto _r = solve(_arr, _k);
        assert(_r == 2);
    }

    return 0;
}
