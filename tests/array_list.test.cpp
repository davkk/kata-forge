#include "kata_test.h"
#include <optional>

int main() { TEST("append_remove_prepend_get_size");
    auto _inst0 = ArrayList{{3}};
    _inst0.append(5);
    _inst0.append(7);
    _inst0.append(9);
    { auto _v = _inst0.get(2); VAL(_v, 9); }
    { auto _v = _inst0.removeAt(1); VAL(_v, 7); }
    EQL(2, _inst0.size());
    _inst0.append(11);
    { auto _v = _inst0.removeAt(1); VAL(_v, 9); }
    { auto _v = _inst0.remove(9); CHECK(!_v.has_value()); }
    { auto _v = _inst0.removeAt(0); VAL(_v, 5); }
    { auto _v = _inst0.removeAt(0); VAL(_v, 11); }
    EQL(0, _inst0.size());
    _inst0.prepend(5);
    _inst0.prepend(7);
    _inst0.prepend(9);
    { auto _v = _inst0.get(2); VAL(_v, 5); }
    { auto _v = _inst0.get(0); VAL(_v, 9); }
    { auto _v = _inst0.remove(9); VAL(_v, 9); }
    EQL(2, _inst0.size());
    { auto _v = _inst0.get(0); VAL(_v, 7); }

    return 0;
}
