#include <cassert>

int main() {
    { // no_cycle
        auto _n0 = new ListNode{1}; _n0->next = _n1;
        auto _n1 = new ListNode{2}; _n1->next = _n2;
        auto _n2 = new ListNode{3};
        ListNode* _head = _n0;
        auto _r = has_cycle(_head);
        assert(_r == false);
    }

    { // cycle
        auto _n0 = new ListNode{1}; _n0->next = _n1;
        auto _n1 = new ListNode{2}; _n1->next = _n2;
        auto _n2 = new ListNode{3};
        ListNode* _head = _n0;
        _n2->next = _n0;
        auto _r = has_cycle(_head);
        assert(_r == true);
    }

    { // empty
        ListNode* _head = nullptr;
        auto _r = has_cycle(_head);
        assert(_r == false);
    }

    { // single
        auto _n0 = new ListNode{1};
        ListNode* _head = _n0;
        auto _r = has_cycle(_head);
        assert(_r == false);
    }

    { // self
        auto _n0 = new ListNode{1};
        ListNode* _head = _n0;
        _n0->next = _n0;
        auto _r = has_cycle(_head);
        assert(_r == true);
    }

    return 0;
}
