#include <cassert>

int main() {
    { // a
        vector<int> _arr = {2, 7, 11, 15};
        int64_t _k = 9;
        auto _r = solve(_arr, _k);
        { auto _exp = vector<int>{1, 2}; assert(_r == _exp); }
    }

    { // b
        vector<int> _arr = {2, 3, 4};
        int64_t _k = 6;
        auto _r = solve(_arr, _k);
        { auto _exp = vector<int>{1, 3}; assert(_r == _exp); }
    }

    { // c
        vector<int> _arr = {-1, 0};
        int64_t _k = -1;
        auto _r = solve(_arr, _k);
        { auto _exp = vector<int>{1, 2}; assert(_r == _exp); }
    }

    { // d
        vector<int> _arr = {1, 2, 3, 4, 5};
        int64_t _k = 9;
        auto _r = solve(_arr, _k);
        { auto _exp = vector<int>{4, 5}; assert(_r == _exp); }
    }

    return 0;
}
