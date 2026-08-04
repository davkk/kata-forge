#include "kata_test.h"

int main() {
    { TEST("all_distinct");
        string _s = "abcdef";
        auto _r = longest_unique_substring(_s);
        EQL(6, _r);
    }

    { TEST("repeated_blocks");
        string _s = "abcabcbb";
        auto _r = longest_unique_substring(_s);
        EQL(3, _r);
    }

    { TEST("single_char");
        string _s = "bbbbb";
        auto _r = longest_unique_substring(_s);
        EQL(1, _r);
    }

    { TEST("empty");
        string _s = "";
        auto _r = longest_unique_substring(_s);
        EQL(0, _r);
    }

    { TEST("repeats_revive");
        string _s = "pwwkew";
        auto _r = longest_unique_substring(_s);
        EQL(3, _r);
    }

    { TEST("tail_is_longest");
        string _s = "abcaab";
        auto _r = longest_unique_substring(_s);
        EQL(3, _r);
    }

    { TEST("mixed_chars");
        string _s = "aA 1b";
        auto _r = longest_unique_substring(_s);
        EQL(5, _r);
    }
}
