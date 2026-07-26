#include "kata_test.h"

int main() {
    { TEST("mst_on_7_node_graph");
        vector<vector<Edge>> _g = {{{1, 3}, {2, 1}}, {{0, 3}, {2, 4}, {4, 1}}, {{1, 4}, {3, 7}, {0, 1}}, {{2, 7}, {4, 5}, {6, 1}}, {{1, 1}, {3, 5}, {5, 2}}, {{6, 1}, {4, 2}, {2, 18}}, {{3, 1}, {5, 1}}};
        auto _r = prim(_g);
        // TODO: assert _r == [[{'to': 2, 'weight': 1}, {'to': 1, 'weight': 3}], [{'to': 0, 'weight': 3}, {'to': 4, 'weight': 1}], [{'to': 0, 'weight': 1}], [{'to': 6, 'weight': 1}], [{'to': 1, 'weight': 1}, {'to': 5, 'weight': 2}], [{'to': 4, 'weight': 2}, {'to': 6, 'weight': 1}], [{'to': 5, 'weight': 1}, {'to': 3, 'weight': 1}]]
    }

    return 0;
}
