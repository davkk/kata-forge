#include <cassert>

int main() {
    { // a
        string _a = "abcxabcdabcdabcy";
        string _b = "abcdabcy";
        auto _r = solve(_a, _b);
        assert(_r == 8);
    }

    { // b
        string _a = "hello world";
        string _b = "world";
        auto _r = solve(_a, _b);
        assert(_r == 6);
    }

    { // c
        string _a = "hello world";
        string _b = "xyz";
        auto _r = solve(_a, _b);
        assert(_r == -1);
    }

    { // d
        string _a = "";
        string _b = "a";
        auto _r = solve(_a, _b);
        assert(_r == -1);
    }

    { // e
        string _a = "a";
        string _b = "";
        auto _r = solve(_a, _b);
        assert(_r == 0);
    }

    { // f
        string _a = "mississippi";
        string _b = "issip";
        auto _r = solve(_a, _b);
        assert(_r == 4);
    }

    { // g
        string _a = "aaaaab";
        string _b = "aaab";
        auto _r = solve(_a, _b);
        assert(_r == 2);
    }

    return 0;
}
