#include "kata_test.h"
#include <optional>

int main() { TEST("set_get_erase_size");
    Map _inst0;
    EQL(0, _inst0.size());
    CHECK(!_inst0.get("foo").has_value());
    _inst0.set("foo", 55);
    _inst0.set("fool", 75);
    _inst0.set("foolish", 105);
    _inst0.set("bar", 69);
    EQL(4, _inst0.size());
    { auto _v = _inst0.get("foo"); VAL(_v, 55); }
    { auto _v = _inst0.get("fool"); VAL(_v, 75); }
    { auto _v = _inst0.get("bar"); VAL(_v, 69); }
    CHECK(!_inst0.get("missing").has_value());
    _inst0.set("foo", 100);
    { auto _v = _inst0.get("foo"); VAL(_v, 100); }
    EQL(4, _inst0.size());
    _inst0.erase("bar");
    EQL(3, _inst0.size());
    CHECK(!_inst0.get("bar").has_value());
    _inst0.erase("missing");
    EQL(3, _inst0.size());

    return 0;
}
