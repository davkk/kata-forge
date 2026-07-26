#include "kata_test.h"
#include <optional>

int main() { TEST("basic");
    MinHeap _inst0;
    CHECK(_inst0.size() == 0);
    _inst0.push(5);
    _inst0.push(3);
    _inst0.push(69);
    _inst0.push(420);
    _inst0.push(4);
    _inst0.push(1);
    _inst0.push(8);
    _inst0.push(7);
    CHECK(_inst0.size() == 8);
    { auto _v = _inst0.pop(); CHECK(_v.has_value() && *_v == 1); }
    { auto _v = _inst0.pop(); CHECK(_v.has_value() && *_v == 3); }
    { auto _v = _inst0.pop(); CHECK(_v.has_value() && *_v == 4); }
    { auto _v = _inst0.pop(); CHECK(_v.has_value() && *_v == 5); }
    CHECK(_inst0.size() == 4);
    { auto _v = _inst0.pop(); CHECK(_v.has_value() && *_v == 7); }
    { auto _v = _inst0.pop(); CHECK(_v.has_value() && *_v == 8); }
    { auto _v = _inst0.pop(); CHECK(_v.has_value() && *_v == 69); }
    { auto _v = _inst0.pop(); CHECK(_v.has_value() && *_v == 420); }
    CHECK(_inst0.size() == 0);

    return 0;
}
