#include "kata_test.h"
#include <optional>

int main() { TEST("basic");
    auto _inst0 = RingBuffer{{3}};
    _inst0.push(5);
    { auto _v = _inst0.pop(); CHECK(_v.has_value() && *_v == 5); }
    { auto _v = _inst0.pop(); CHECK(!_v.has_value()); }
    _inst0.push(42);
    _inst0.push(9);
    { auto _v = _inst0.pop(); CHECK(_v.has_value() && *_v == 42); }
    { auto _v = _inst0.pop(); CHECK(_v.has_value() && *_v == 9); }
    { auto _v = _inst0.pop(); CHECK(!_v.has_value()); }
    _inst0.push(42);
    _inst0.push(9);
    _inst0.push(12);
    { auto _v = _inst0.get(2); CHECK(_v.has_value() && *_v == 12); }
    { auto _v = _inst0.get(1); CHECK(_v.has_value() && *_v == 9); }
    { auto _v = _inst0.get(0); CHECK(_v.has_value() && *_v == 42); }

    return 0;
}
