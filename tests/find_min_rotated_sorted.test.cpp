#include <cassert>

int main() {
    { // a
        vector<int> _arr = {3, 4, 5, 1, 2};
        auto _r = solve(_arr);
        assert(_r == 1);
    }

    { // b
        vector<int> _arr = {4, 5, 6, 7, 0, 1, 2};
        auto _r = solve(_arr);
        assert(_r == 0);
    }

    { // c
        vector<int> _arr = {11, 13, 15, 17};
        auto _r = solve(_arr);
        assert(_r == 11);
    }

    { // d
        vector<int> _arr = {1};
        auto _r = solve(_arr);
        assert(_r == 1);
    }

    { // e
        vector<int> _arr = {2, 1};
        auto _r = solve(_arr);
        assert(_r == 1);
    }

    return 0;
}
