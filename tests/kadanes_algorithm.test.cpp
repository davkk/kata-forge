#include <cassert>

int main() {
    { // a
        vector<int> _arr = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
        auto _r = solve(_arr);
        assert(_r == 6);
    }

    { // b
        vector<int> _arr = {1};
        auto _r = solve(_arr);
        assert(_r == 1);
    }

    { // c
        vector<int> _arr = {5, 4, -1, 7, 8};
        auto _r = solve(_arr);
        assert(_r == 23);
    }

    { // d
        vector<int> _arr = {-1};
        auto _r = solve(_arr);
        assert(_r == -1);
    }

    { // e
        vector<int> _arr = {-2, -1};
        auto _r = solve(_arr);
        assert(_r == -1);
    }

    return 0;
}
