#include <cassert>
#include <optional>

int main() {
    // basic
    auto _inst0 = LRU{{3}};
    { auto _v = _inst0.get("foo"); assert(!_v.has_value()); }
    _inst0.update("foo", 69);
    { auto _v = _inst0.get("foo"); assert(_v.has_value() && *_v == 69); }
    _inst0.update("bar", 420);
    { auto _v = _inst0.get("bar"); assert(_v.has_value() && *_v == 420); }
    _inst0.update("baz", 1337);
    { auto _v = _inst0.get("baz"); assert(_v.has_value() && *_v == 1337); }
    _inst0.update("ball", 69420);
    { auto _v = _inst0.get("ball"); assert(_v.has_value() && *_v == 69420); }
    { auto _v = _inst0.get("foo"); assert(!_v.has_value()); }
    { auto _v = _inst0.get("bar"); assert(_v.has_value() && *_v == 420); }
    _inst0.update("foo", 69);
    { auto _v = _inst0.get("bar"); assert(_v.has_value() && *_v == 420); }
    { auto _v = _inst0.get("foo"); assert(_v.has_value() && *_v == 69); }
    { auto _v = _inst0.get("baz"); assert(!_v.has_value()); }

    return 0;
}
