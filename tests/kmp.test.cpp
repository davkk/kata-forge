#include "kata_test.h"

int main() {
    { TEST("standard_match");
        string _a = "abcxabcdabcdabcy";
        string _b = "abcdabcy";
        auto _r = kmp(_a, _b);
        CHECK(_r == 8);
    }

    { TEST("suffix_match");
        string _a = "hello world";
        string _b = "world";
        auto _r = kmp(_a, _b);
        CHECK(_r == 6);
    }

    { TEST("no_match");
        string _a = "hello world";
        string _b = "xyz";
        auto _r = kmp(_a, _b);
        CHECK(_r == -1);
    }

    { TEST("empty_text");
        string _a = "";
        string _b = "a";
        auto _r = kmp(_a, _b);
        CHECK(_r == -1);
    }

    { TEST("empty_pattern");
        string _a = "a";
        string _b = "";
        auto _r = kmp(_a, _b);
        CHECK(_r == 0);
    }

    { TEST("repeated_chars_match");
        string _a = "mississippi";
        string _b = "issip";
        auto _r = kmp(_a, _b);
        CHECK(_r == 4);
    }

    { TEST("prefix_repeated");
        string _a = "aaaaab";
        string _b = "aaab";
        auto _r = kmp(_a, _b);
        CHECK(_r == 2);
    }

    return 0;
}
