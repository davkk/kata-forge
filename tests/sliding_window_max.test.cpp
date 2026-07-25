#include <cassert>

int main() {
    { // a
        vector<int> _arr = {1, 3, -1, -3, 5, 3, 6, 7};
        int64_t _k = 3;
        auto _r = solve(_arr, _k);
        { auto _exp = vector<int>{3, 3, 5, 5, 6, 7}; assert(_r == _exp); }
    }

    { // b
        vector<int> _arr = {1};
        int64_t _k = 1;
        auto _r = solve(_arr, _k);
        { auto _exp = vector<int>{1}; assert(_r == _exp); }
    }

    { // c
        vector<int> _arr = {1, -1};
        int64_t _k = 1;
        auto _r = solve(_arr, _k);
        { auto _exp = vector<int>{1, -1}; assert(_r == _exp); }
    }

    { // d
        vector<int> _arr = {9, 11};
        int64_t _k = 2;
        auto _r = solve(_arr, _k);
        { auto _exp = vector<int>{11}; assert(_r == _exp); }
    }

    { // e
        vector<int> _arr = {4, -2};
        int64_t _k = 2;
        auto _r = solve(_arr, _k);
        { auto _exp = vector<int>{4}; assert(_r == _exp); }
    }

    return 0;
}
