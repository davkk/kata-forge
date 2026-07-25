#include <cassert>
#include <optional>

int main() {
    // basic
    auto _inst0 = RingBuffer{{3}};
    _inst0.push(5);
    { auto _v = _inst0.pop(); assert(_v.has_value() && *_v == 5); }
    { auto _v = _inst0.pop(); assert(!_v.has_value()); }
    _inst0.push(42);
    _inst0.push(9);
    { auto _v = _inst0.pop(); assert(_v.has_value() && *_v == 42); }
    { auto _v = _inst0.pop(); assert(_v.has_value() && *_v == 9); }
    { auto _v = _inst0.pop(); assert(!_v.has_value()); }
    _inst0.push(42);
    _inst0.push(9);
    _inst0.push(12);
    { auto _v = _inst0.get(2); assert(_v.has_value() && *_v == 12); }
    { auto _v = _inst0.get(1); assert(_v.has_value() && *_v == 9); }
    { auto _v = _inst0.get(0); assert(_v.has_value() && *_v == 42); }

    return 0;
}
