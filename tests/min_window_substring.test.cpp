#include "kata_test.h"

int main() {
    { TEST("classic");
        string _s = "ADOBECODEBANC";
        string _t = "ABC";
        auto _r = min_window_substring(_s, _t);
        EQL(string("BANC"), _r);
    }

    { TEST("window_uses_duplicate");
        string _s = "aabc";
        string _t = "aa";
        auto _r = min_window_substring(_s, _t);
        EQL(string("aa"), _r);
    }

    { TEST("single_char");
        string _s = "ab";
        string _t = "b";
        auto _r = min_window_substring(_s, _t);
        EQL(string("b"), _r);
    }

    { TEST("no_window");
        string _s = "abc";
        string _t = "abcd";
        auto _r = min_window_substring(_s, _t);
        EQL(string(""), _r);
    }

    { TEST("whole_string");
        string _s = "a";
        string _t = "a";
        auto _r = min_window_substring(_s, _t);
        EQL(string("a"), _r);
    }

    { TEST("shortest_wins");
        string _s = "abbbac";
        string _t = "ab";
        auto _r = min_window_substring(_s, _t);
        EQL(string("ab"), _r);
    }

    { TEST("t_has_all_dups");
        string _s = "xxyxxz";
        string _t = "xx";
        auto _r = min_window_substring(_s, _t);
        EQL(string("xx"), _r);
    }
}
