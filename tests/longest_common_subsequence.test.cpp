#include "kata_test.h"

int main() {
    { TEST("noncontiguous_match");
        string _a = "abcde";
        string _b = "ace";
        auto _r = longest_common_subsequence(_a, _b);
        CHECK(_r == 3);
    }

    { TEST("identical_strings");
        string _a = "abc";
        string _b = "abc";
        auto _r = longest_common_subsequence(_a, _b);
        CHECK(_r == 3);
    }

    { TEST("no_common");
        string _a = "abc";
        string _b = "def";
        auto _r = longest_common_subsequence(_a, _b);
        CHECK(_r == 0);
    }

    { TEST("empty_string");
        string _a = "";
        string _b = "abc";
        auto _r = longest_common_subsequence(_a, _b);
        CHECK(_r == 0);
    }

    { TEST("standard_example");
        string _a = "AGGTAB";
        string _b = "GXTXAYB";
        auto _r = longest_common_subsequence(_a, _b);
        CHECK(_r == 4);
    }

    return 0;
}
