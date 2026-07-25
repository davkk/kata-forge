#include <cassert>

int main() {
    { // a
        string _a = "abcde";
        string _b = "ace";
        auto _r = solve(_a, _b);
        assert(_r == 3);
    }

    { // b
        string _a = "abc";
        string _b = "abc";
        auto _r = solve(_a, _b);
        assert(_r == 3);
    }

    { // c
        string _a = "abc";
        string _b = "def";
        auto _r = solve(_a, _b);
        assert(_r == 0);
    }

    { // d
        string _a = "";
        string _b = "abc";
        auto _r = solve(_a, _b);
        assert(_r == 0);
    }

    { // e
        string _a = "AGGTAB";
        string _b = "GXTXAYB";
        auto _r = solve(_a, _b);
        assert(_r == 4);
    }

    return 0;
}
