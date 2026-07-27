#include "kata_test.h"
#include <optional>

int main() { TEST("insert_and_extract_min_in_order");
    MinHeap _inst0;
    CHECK(_inst0.length() == 0);
    _inst0.insert(5);
    _inst0.insert(3);
    _inst0.insert(69);
    _inst0.insert(420);
    _inst0.insert(4);
    _inst0.insert(1);
    _inst0.insert(8);
    _inst0.insert(7);
    CHECK(_inst0.length() == 8);
    { auto _v = _inst0.deleteMin(); CHECK(_v.has_value() && *_v == 1); }
    { auto _v = _inst0.deleteMin(); CHECK(_v.has_value() && *_v == 3); }
    { auto _v = _inst0.deleteMin(); CHECK(_v.has_value() && *_v == 4); }
    { auto _v = _inst0.deleteMin(); CHECK(_v.has_value() && *_v == 5); }
    CHECK(_inst0.length() == 4);
    { auto _v = _inst0.deleteMin(); CHECK(_v.has_value() && *_v == 7); }
    { auto _v = _inst0.deleteMin(); CHECK(_v.has_value() && *_v == 8); }
    { auto _v = _inst0.deleteMin(); CHECK(_v.has_value() && *_v == 69); }
    { auto _v = _inst0.deleteMin(); CHECK(_v.has_value() && *_v == 420); }
    CHECK(_inst0.length() == 0);

    return 0;
}
