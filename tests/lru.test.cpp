#include "kata_test.h"
#include <optional>

int main() { TEST("evict_lru_on_overflow");
    auto _inst0 = LRU{{3}};
    { auto _v = _inst0.get("foo"); CHECK(!_v.has_value()); }
    _inst0.update("foo", 69);
    { auto _v = _inst0.get("foo"); VAL(_v, 69); }
    _inst0.update("bar", 420);
    { auto _v = _inst0.get("bar"); VAL(_v, 420); }
    _inst0.update("baz", 1337);
    { auto _v = _inst0.get("baz"); VAL(_v, 1337); }
    _inst0.update("ball", 69420);
    { auto _v = _inst0.get("ball"); VAL(_v, 69420); }
    { auto _v = _inst0.get("foo"); CHECK(!_v.has_value()); }
    { auto _v = _inst0.get("bar"); VAL(_v, 420); }
    _inst0.update("foo", 69);
    { auto _v = _inst0.get("bar"); VAL(_v, 420); }
    { auto _v = _inst0.get("foo"); VAL(_v, 69); }
    { auto _v = _inst0.get("baz"); CHECK(!_v.has_value()); }

    { TEST("size");
        auto _inst1 = LRU{{2}};
        EQL(0, _inst1.size());
        _inst1.update("a", 1);
        _inst1.update("b", 2);
        EQL(2, _inst1.size());
        _inst1.update("c", 3);
        EQL(2, _inst1.size());
        _inst1.update("b", 22);
        EQL(2, _inst1.size());
    }

    return 0;
}
