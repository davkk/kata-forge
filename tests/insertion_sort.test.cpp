#include <cassert>

int main() {
    { // basic
        vector<int> _arr = {9, 3, 7, 4, 69, 420, 42};
        solve(_arr);
        { auto _exp = vector<int>{3, 4, 7, 9, 42, 69, 420}; assert(_arr == _exp); }
    }

    { // already_sorted
        vector<int> _arr = {1, 2, 3};
        solve(_arr);
        { auto _exp = vector<int>{1, 2, 3}; assert(_arr == _exp); }
    }

    { // reverse
        vector<int> _arr = {5, 4, 3, 2, 1};
        solve(_arr);
        { auto _exp = vector<int>{1, 2, 3, 4, 5}; assert(_arr == _exp); }
    }

    { // single
        vector<int> _arr = {42};
        solve(_arr);
        { auto _exp = vector<int>{42}; assert(_arr == _exp); }
    }

    { // empty
        vector<int> _arr = {};
        solve(_arr);
        { auto _exp = vector<int>{}; assert(_arr == _exp); }
    }

    { // duplicates
        vector<int> _arr = {3, 1, 2, 1, 3};
        solve(_arr);
        { auto _exp = vector<int>{1, 1, 2, 3, 3}; assert(_arr == _exp); }
    }

    return 0;
}
