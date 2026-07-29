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

    { TEST("insertAt");
        auto _inst1 = ArrayList{{5}};
        _inst1.append(1);
        _inst1.append(3);
        _inst1.insertAt(1, 2);
        { auto _v = _inst1.get(1); VAL(_v, 2); }
        { auto _v = _inst1.get(2); VAL(_v, 3); }
        EQL(3, _inst1.size());
        _inst1.insertAt(0, 0);
        { auto _v = _inst1.get(0); VAL(_v, 0); }
        EQL(4, _inst1.size());
    }

    return 0;
}
