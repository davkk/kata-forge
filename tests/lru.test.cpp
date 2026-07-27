#include "kata_test.h"
#include <optional>

int main() { TEST("evict_lru_on_overflow");
    auto _inst0 = LRU{{3}};
    { auto _v = _inst0.get("foo"); CHECK(!_v.has_value()); }
    _inst0.update("foo", 69);
    { auto _v = _inst0.get("foo"); CHECK(_v.has_value() && *_v == 69); }
    _inst0.update("bar", 420);
    { auto _v = _inst0.get("bar"); CHECK(_v.has_value() && *_v == 420); }
    _inst0.update("baz", 1337);
    { auto _v = _inst0.get("baz"); CHECK(_v.has_value() && *_v == 1337); }
    _inst0.update("ball", 69420);
    { auto _v = _inst0.get("ball"); CHECK(_v.has_value() && *_v == 69420); }
    { auto _v = _inst0.get("foo"); CHECK(!_v.has_value()); }
    { auto _v = _inst0.get("bar"); CHECK(_v.has_value() && *_v == 420); }
    _inst0.update("foo", 69);
    { auto _v = _inst0.get("bar"); CHECK(_v.has_value() && *_v == 420); }
    { auto _v = _inst0.get("foo"); CHECK(_v.has_value() && *_v == 69); }
    { auto _v = _inst0.get("baz"); CHECK(!_v.has_value()); }

    return 0;
}
