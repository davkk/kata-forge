#include "kata_test.h"

int main() {
    { TEST("standard_match");
        string _a = "abcxabcdabcdabcy";
        string _b = "abcdabcy";
        auto _r = kmp_search(_a, _b);
        EQL(8, _r);
    }

    { TEST("suffix_match");
        string _a = "hello world";
        string _b = "world";
        auto _r = kmp_search(_a, _b);
        EQL(6, _r);
    }

    { TEST("no_match");
        string _a = "hello world";
        string _b = "xyz";
        auto _r = kmp_search(_a, _b);
        EQL(-1, _r);
    }

    { TEST("empty_text");
        string _a = "";
        string _b = "a";
        auto _r = kmp_search(_a, _b);
        EQL(-1, _r);
    }

    { TEST("empty_pattern");
        string _a = "a";
        string _b = "";
        auto _r = kmp_search(_a, _b);
        EQL(0, _r);
    }

    { TEST("repeated_chars_match");
        string _a = "mississippi";
        string _b = "issip";
        auto _r = kmp_search(_a, _b);
        EQL(4, _r);
    }

    { TEST("prefix_repeated");
        string _a = "aaaaab";
        string _b = "aaab";
        auto _r = kmp_search(_a, _b);
        EQL(2, _r);
    }

    return 0;
}
