# Shapes

Wire formats spoken by `lib/*/` and `./test`. Catalog tests use structured JSON; the judge encodes/decodes these texts.

## `list_inplace`

**stdin:** `n` then `n` ints · **stdout:** sorted ints  
**C++:** `run_list_inplace([](vector<int64_t>& a){...})`

## `commands`

**stdin:**
```
init_n [init args...]   # 0 if none
op_count
op [args...]            # ints bare; strings "quoted"
```
**stdout:** one line per valued reply (`empty` / `true`/`false` / int / float / string; string-lists: `count` then lines).

**C++:** `run_commands([](const vector<int64_t>& init, const vector<Op>& ops) -> vector<Reply>)`

## `search`

**stdin:** list + `needle` · **stdout:** `true`/`false`  
**C++:** `run_search`

## `list_scalar` / `list_k_scalar`

**stdin:** list [+ `k`] · **stdout:** one int  
**C++:** `run_list_scalar` / `run_list_k_scalar`

## `list_list` / `list_k_list`

**stdin:** list [+ `k`] · **stdout:** list  
**C++:** `run_list_list` / `run_list_k_list`

## `pairs`

**stdin:** `n` then `n` lines `a b` · **stdout:** same  
**C++:** `run_pairs`

## `coin`

**stdin:** `amount` + coin list · **stdout:** int  
**C++:** `run_coin`

## `knapsack`

**stdin:** `capacity` + weights list + values list · **stdout:** int  
**C++:** `run_knapsack`

## `str2_scalar`

**stdin:** two length-prefixed strings (`len content`) · **stdout:** int  
**C++:** `run_str2_scalar`

## Trees

Encoding: `0` = null; `1 value` then left then right (pre-order flags).

| shape | I/O | C++ |
|-------|-----|-----|
| `tree_list` | tree → list | `run_tree_list` |
| `tree_find` | tree + needle → bool | `run_tree_find` |
| `tree_eq` | two trees → bool | `run_tree_eq` |
| `tree_map` | tree → tree | `run_tree_map` |

## Graphs

**Adj list stdin:**
```
n
m to w to w ...     # per node
```

| shape | I/O | C++ |
|-------|-----|-----|
| `graph_path` | adj + source sink → path or `null` | `run_graph_path` |
| `matrix_path` | `n` + n×n matrix + source sink → path/`null` | `run_matrix_path` |
| `graph_mst` | adj → adj or `null` | `run_graph_mst` |
| `topo` | `n` + edge pairs → list | `run_topo` |

## `list_lists` / `combos`

**list_lists:** list → `count` then rows `m v1..vm` (order-insensitive judge)  
**combos:** `n k` → same  
**C++:** `run_list_lists` / `run_combos`

## `maze`

**stdin:** `rows cols`, row lines, wall token, `sx sy ex ey`  
**stdout:** `k` then `k` lines `x y`  
**C++:** `run_maze`

## `cycle`

**stdin:** list values + `cycle_to` index (`-1` = none)  
**stdout:** `true`/`false`  
**C++:** `run_cycle`
