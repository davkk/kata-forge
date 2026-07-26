#include "kata_test.h"

int main() {
    { TEST("no_cycle");
        auto _n0 = new ListNode{1}; _n0->next = _n1;
        auto _n1 = new ListNode{2}; _n1->next = _n2;
        auto _n2 = new ListNode{3};
        ListNode* _head = _n0;
        auto _r = has_cycle(_head);
        CHECK(_r == false);
    }

    { TEST("cycle");
        auto _n0 = new ListNode{1}; _n0->next = _n1;
        auto _n1 = new ListNode{2}; _n1->next = _n2;
        auto _n2 = new ListNode{3};
        ListNode* _head = _n0;
        _n2->next = _n0;
        auto _r = has_cycle(_head);
        CHECK(_r == true);
    }

    { TEST("empty");
        ListNode* _head = nullptr;
        auto _r = has_cycle(_head);
        CHECK(_r == false);
    }

    { TEST("single");
        auto _n0 = new ListNode{1};
        ListNode* _head = _n0;
        auto _r = has_cycle(_head);
        CHECK(_r == false);
    }

    { TEST("self");
        auto _n0 = new ListNode{1};
        ListNode* _head = _n0;
        _n0->next = _n0;
        auto _r = has_cycle(_head);
        CHECK(_r == true);
    }

    return 0;
}
