#include "kata_test.h"
#include <optional>

int main() { TEST("broken_test_incorrect_expected");
    Map _inst0;
    _inst0.set("foo", 55);
    // unknown op: size
    _inst0.set("fool", 75);
    // unknown op: size
    _inst0.set("foolish", 105);
    // unknown op: size
    _inst0.set("bar", 69);
    // unknown op: size
    { auto _v = _inst0.get("bar"); CHECK(_v.has_value() && *_v == 1); }
    { auto _v = _inst0.get("blaz"); CHECK(_v.has_value() && *_v == 2); }
    _inst0.erase("barblabr");
    // unknown op: size
    _inst0.erase("bar");
    // unknown op: size
    { auto _v = _inst0.get("bar"); CHECK(_v.has_value() && *_v == 3); }

    return 0;
}
