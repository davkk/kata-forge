#include <cassert>

int main() {
    { // a
        vector<int> _arr = {73, 74, 75, 71, 69, 72, 76, 73};
        auto _r = solve(_arr);
        { auto _exp = vector<int>{1, 1, 4, 2, 1, 1, 0, 0}; assert(_r == _exp); }
    }

    { // b
        vector<int> _arr = {30, 40, 50, 60};
        auto _r = solve(_arr);
        { auto _exp = vector<int>{1, 1, 1, 0}; assert(_r == _exp); }
    }

    { // c
        vector<int> _arr = {30, 60, 90};
        auto _r = solve(_arr);
        { auto _exp = vector<int>{1, 1, 0}; assert(_r == _exp); }
    }

    { // d
        vector<int> _arr = {90, 80, 70};
        auto _r = solve(_arr);
        { auto _exp = vector<int>{0, 0, 0}; assert(_r == _exp); }
    }

    return 0;
}
