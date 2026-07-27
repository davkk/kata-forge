#include "kata_test.h"

static bool eq_edge(const Edge& a, const Edge& b) {
    return a.to == b.to && a.weight == b.weight;
}

int main() {
    { TEST("mst_on_weighted_graph");
        vector<vector<Edge>> _g = {{{1, 3}, {2, 1}}, {{0, 3}, {2, 4}, {4, 1}}, {{1, 4}, {3, 7}, {0, 1}}, {{2, 7}, {4, 5}, {6, 1}}, {{1, 1}, {3, 5}, {5, 2}}, {{6, 1}, {4, 2}, {2, 18}}, {{3, 1}, {5, 1}}};
        auto _r = kruskals(_g);
        CHECK(_r.has_value());
        auto& _m = *_r;
        vector<vector<Edge>> _exp = {
            {{2, 1}, {1, 3}}, {{4, 1}, {0, 3}}, {{0, 1}},
            {{6, 1}}, {{1, 1}, {5, 2}}, {{6, 1}, {4, 2}},
            {{3, 1}, {5, 1}}
        };
        CHECK(_m.size() == _exp.size());
        for (int i = 0; i < (int)_m.size(); ++i) {
            CHECK(_m[i].size() == _exp[i].size());
            for (int j = 0; j < (int)_m[i].size(); ++j)
                CHECK(eq_edge(_m[i][j], _exp[i][j]));
        }
    }

    return 0;
}
