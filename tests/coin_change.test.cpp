#include <cassert>

int main() {
    { // a
        int64_t _amount = 5;
        vector<int> _coins = {1, 2, 5};
        auto _r = solve(_amount, _coins);
        assert(_r == 1);
    }

    { // b
        int64_t _amount = 3;
        vector<int> _coins = {2};
        auto _r = solve(_amount, _coins);
        assert(_r == -1);
    }

    { // c
        int64_t _amount = 0;
        vector<int> _coins = {1, 2, 5};
        auto _r = solve(_amount, _coins);
        assert(_r == 0);
    }

    { // d
        int64_t _amount = 11;
        vector<int> _coins = {1, 2, 5};
        auto _r = solve(_amount, _coins);
        assert(_r == 3);
    }

    { // e
        int64_t _amount = 100;
        vector<int> _coins = {1, 10, 25};
        auto _r = solve(_amount, _coins);
        assert(_r == 4);
    }

    return 0;
}
