#include "kata_test.h"

int main() {
    { TEST("noncontiguous_match");
        string _a = "abcde";
        string _b = "ace";
        auto _r = lcs(_a, _b);
        EQL(3, _r);
    }

    { TEST("identical_strings");
        string _a = "abc";
        string _b = "abc";
        auto _r = lcs(_a, _b);
        EQL(3, _r);
    }

    { TEST("no_common");
        string _a = "abc";
        string _b = "def";
        auto _r = lcs(_a, _b);
        EQL(0, _r);
    }

    { TEST("empty_string");
        string _a = "";
        string _b = "abc";
        auto _r = lcs(_a, _b);
        EQL(0, _r);
    }

    { TEST("standard_example");
        string _a = "AGGTAB";
        string _b = "GXTXAYB";
        auto _r = lcs(_a, _b);
        EQL(4, _r);
    }

    return 0;
}
