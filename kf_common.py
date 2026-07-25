#!/usr/bin/env python3
"""Shared helpers for kata-forge CLIs."""

from __future__ import annotations

import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
CATALOG_DIR = ROOT / "catalog"
LIB_CPP = ROOT / "lib" / "cpp"
SESSIONS_DIR = ROOT / "sessions"
ACTIVE_PATH = ROOT / ".active.json"

SHAPES = {
    "list_inplace",
    "commands",
    "search",
    "list_scalar",
    "list_k_scalar",
    "list_list",
    "list_k_list",
    "pairs",
    "coin",
    "knapsack",
    "str2_scalar",
    "tree_list",
    "tree_find",
    "tree_eq",
    "tree_map",
    "graph_path",
    "matrix_path",
    "graph_mst",
    "topo",
    "list_lists",
    "combos",
    "maze",
    "cycle",
}


def sessions_dir() -> Path:
    return SESSIONS_DIR


def active_path() -> Path:
    return ACTIVE_PATH


def ensure_sessions_dir() -> None:
    SESSIONS_DIR.mkdir(parents=True, exist_ok=True)


def load_catalog(path: Path) -> dict:
    return json.loads(path.read_text(encoding="utf-8"))


def list_katas() -> list[dict]:
    katas = []
    for path in sorted(CATALOG_DIR.glob("*.json")):
        data = load_catalog(path)
        if not data or "id" not in data:
            continue
        data["_path"] = path
        katas.append(data)
    return katas


def kata_by_id(kid: str) -> dict | None:
    path = CATALOG_DIR / f"{kid}.json"
    if not path.exists():
        return None
    data = load_catalog(path)
    data["_path"] = path
    return data


def read_json(path: Path, fallback):
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError):
        return fallback


def write_json(path: Path, data) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(data, indent=2) + "\n", encoding="utf-8")


def latest_session() -> Path | None:
    d = sessions_dir()
    if not d.exists():
        return None
    sessions = [p for p in d.iterdir() if p.is_dir()]
    if not sessions:
        return None
    return max(sessions, key=lambda p: p.name)


def new_session_id() -> str:
    ensure_sessions_dir()
    nums = []
    for p in SESSIONS_DIR.iterdir():
        if p.is_dir() and p.name.startswith("session"):
            suffix = p.name[len("session") :]
            if suffix.isdigit():
                nums.append(int(suffix))
    n = (max(nums) + 1) if nums else 1
    return f"session{n}"


# --- encode helpers ----------------------------------------------------------


def _enc_list_i64(arr) -> list[str]:
    arr = arr or []
    lines = [str(len(arr))]
    if arr:
        lines.append(" ".join(str(int(x)) for x in arr))
    return lines


def _enc_pairs(pairs) -> list[str]:
    pairs = pairs or []
    lines = [str(len(pairs))]
    for a, b in pairs:
        lines.append(f"{int(a)} {int(b)}")
    return lines


def _enc_string(s: str) -> str:
    s = s if s is not None else ""
    return f"{len(s)} {s}"


def _enc_tree(node) -> list[str]:
    """node: None or {value, left, right}"""
    if node is None:
        return ["0"]
    lines = [f"1 {int(node['value'])}"]
    lines.extend(_enc_tree(node.get("left")))
    lines.extend(_enc_tree(node.get("right")))
    return lines


def _enc_adj(g) -> list[str]:
    """g: list of list of {to, weight}"""
    g = g or []
    lines = [str(len(g))]
    for row in g:
        row = row or []
        parts = [str(len(row))]
        for e in row:
            parts.append(str(int(e["to"])))
            parts.append(str(int(e["weight"])))
        lines.append(" ".join(parts))
    return lines


def _enc_matrix(m) -> list[str]:
    m = m or []
    lines = [str(len(m))]
    for row in m:
        lines.append(" ".join(str(int(x)) for x in row))
    return lines


def _quote_arg(a) -> str:
    if isinstance(a, bool):
        return "1" if a else "0"
    if isinstance(a, (int, float)) and not isinstance(a, bool):
        return str(int(a)) if float(a) == int(a) else str(a)
    s = str(a)
    if any(c.isspace() for c in s) or s == "" or not s.replace("_", "").isalnum():
        return '"' + s.replace('"', '\\"') + '"'
    # simple tokens can stay bare; still quote non-numeric non-alnum-ish keys with mixed
    if s.isidentifier() or s.replace("_", "").isalnum():
        return '"' + s + '"'
    return '"' + s.replace('"', '\\"') + '"'


def encode_list_inplace(case_in: dict) -> str:
    return "\n".join(_enc_list_i64(case_in.get("arr"))) + "\n"


def encode_commands(case_in: dict) -> str:
    init = case_in.get("init") or []
    lines = []
    if init:
        lines.append(str(len(init)) + " " + " ".join(str(int(x)) for x in init))
    else:
        lines.append("0")
    ops = case_in.get("ops") or []
    lines.append(str(len(ops)))
    for op in ops:
        name = op["op"]
        parts = [name]
        for a in op.get("args") or []:
            if isinstance(a, str):
                parts.append(_quote_arg(a))
            else:
                parts.append(str(int(a)))
        # allow separate sargs
        for a in op.get("sargs") or []:
            parts.append(_quote_arg(a))
        lines.append(" ".join(parts))
    return "\n".join(lines) + "\n"


def encode_search(case_in: dict) -> str:
    lines = _enc_list_i64(case_in.get("arr"))
    lines.append(str(int(case_in["needle"])))
    return "\n".join(lines) + "\n"


def encode_list_scalar(case_in: dict) -> str:
    return "\n".join(_enc_list_i64(case_in.get("arr"))) + "\n"


def encode_list_k_scalar(case_in: dict) -> str:
    lines = _enc_list_i64(case_in.get("arr"))
    lines.append(str(int(case_in["k"])))
    return "\n".join(lines) + "\n"


def encode_list_list(case_in: dict) -> str:
    return "\n".join(_enc_list_i64(case_in.get("arr"))) + "\n"


def encode_list_k_list(case_in: dict) -> str:
    lines = _enc_list_i64(case_in.get("arr"))
    lines.append(str(int(case_in["k"])))
    return "\n".join(lines) + "\n"


def encode_pairs(case_in: dict) -> str:
    return "\n".join(_enc_pairs(case_in.get("pairs"))) + "\n"


def encode_coin(case_in: dict) -> str:
    lines = [str(int(case_in["amount"]))]
    lines.extend(_enc_list_i64(case_in.get("coins")))
    return "\n".join(lines) + "\n"


def encode_knapsack(case_in: dict) -> str:
    lines = [str(int(case_in["capacity"]))]
    lines.extend(_enc_list_i64(case_in.get("weights")))
    lines.extend(_enc_list_i64(case_in.get("values")))
    return "\n".join(lines) + "\n"


def encode_str2_scalar(case_in: dict) -> str:
    return _enc_string(case_in.get("a", "")) + "\n" + _enc_string(case_in.get("b", "")) + "\n"


def encode_tree_list(case_in: dict) -> str:
    return "\n".join(_enc_tree(case_in.get("tree"))) + "\n"


def encode_tree_find(case_in: dict) -> str:
    lines = _enc_tree(case_in.get("tree"))
    lines.append(str(int(case_in["needle"])))
    return "\n".join(lines) + "\n"


def encode_tree_eq(case_in: dict) -> str:
    lines = _enc_tree(case_in.get("a"))
    lines.extend(_enc_tree(case_in.get("b")))
    return "\n".join(lines) + "\n"


def encode_tree_map(case_in: dict) -> str:
    return "\n".join(_enc_tree(case_in.get("tree"))) + "\n"


def encode_graph_path(case_in: dict) -> str:
    lines = _enc_adj(case_in.get("graph"))
    lines.append(f"{int(case_in['source'])} {int(case_in['sink'])}")
    return "\n".join(lines) + "\n"


def encode_matrix_path(case_in: dict) -> str:
    lines = _enc_matrix(case_in.get("graph"))
    lines.append(f"{int(case_in['source'])} {int(case_in['sink'])}")
    return "\n".join(lines) + "\n"


def encode_graph_mst(case_in: dict) -> str:
    return "\n".join(_enc_adj(case_in.get("graph"))) + "\n"


def encode_topo(case_in: dict) -> str:
    lines = [str(int(case_in["n"]))]
    lines.extend(_enc_pairs(case_in.get("edges")))
    return "\n".join(lines) + "\n"


def encode_list_lists(case_in: dict) -> str:
    return "\n".join(_enc_list_i64(case_in.get("arr"))) + "\n"


def encode_combos(case_in: dict) -> str:
    return f"{int(case_in['n'])} {int(case_in['k'])}\n"


def encode_maze(case_in: dict) -> str:
    maze = case_in["maze"]
    rows = len(maze)
    cols = len(maze[0]) if maze else 0
    lines = [f"{rows} {cols}"]
    lines.extend(maze)
    wall = case_in.get("wall", "x")
    s = case_in["start"]
    e = case_in["end"]
    lines.append(wall)
    lines.append(f"{int(s['x'])} {int(s['y'])} {int(e['x'])} {int(e['y'])}")
    return "\n".join(lines) + "\n"


def encode_cycle(case_in: dict) -> str:
    vals = case_in.get("arr") or []
    lines = [str(len(vals))]
    if vals:
        lines.append(" ".join(str(int(x)) for x in vals))
    lines.append(str(int(case_in.get("cycle_to", -1))))
    return "\n".join(lines) + "\n"


_ENCODERS = {
    "list_inplace": encode_list_inplace,
    "commands": encode_commands,
    "search": encode_search,
    "list_scalar": encode_list_scalar,
    "list_k_scalar": encode_list_k_scalar,
    "list_list": encode_list_list,
    "list_k_list": encode_list_k_list,
    "pairs": encode_pairs,
    "coin": encode_coin,
    "knapsack": encode_knapsack,
    "str2_scalar": encode_str2_scalar,
    "tree_list": encode_tree_list,
    "tree_find": encode_tree_find,
    "tree_eq": encode_tree_eq,
    "tree_map": encode_tree_map,
    "graph_path": encode_graph_path,
    "matrix_path": encode_matrix_path,
    "graph_mst": encode_graph_mst,
    "topo": encode_topo,
    "list_lists": encode_list_lists,
    "combos": encode_combos,
    "maze": encode_maze,
    "cycle": encode_cycle,
}


def encode_case(shape: str, case_in) -> str:
    if shape not in _ENCODERS:
        raise ValueError(f"unknown shape: {shape}")
    return _ENCODERS[shape](case_in)


# --- decode helpers ----------------------------------------------------------


def decode_list_inplace(stdout: str) -> dict:
    text = stdout.strip()
    if text == "":
        return {"arr": []}
    parts = text.replace("\n", " ").split()
    return {"arr": [int(x) for x in parts]}


def _parse_reply_stream(lines: list[str], i: int = 0):
    """Parse one reply token stream starting at lines[i]. Returns (value, next_i)."""
    if i >= len(lines):
        return None, i
    line = lines[i]
    if line == "empty":
        return "empty", i + 1
    if line == "true":
        return True, i + 1
    if line == "false":
        return False, i + 1
    # string list: integer count then that many lines (heuristic — used when expected wants list)
    return line, i + 1


def decode_commands(stdout: str, expected=None) -> list:
    """Decode valued-op stdout lines into a list comparable to catalog out."""
    raw_lines = [ln.strip() for ln in stdout.splitlines() if ln.strip() != ""]
    if expected is None:
        # best-effort generic
        result = []
        i = 0
        while i < len(raw_lines):
            line = raw_lines[i]
            if line == "empty":
                result.append("empty")
                i += 1
            elif line == "true":
                result.append(True)
                i += 1
            elif line == "false":
                result.append(False)
                i += 1
            else:
                try:
                    # could be start of strs list if next lines are strings — leave as int/float/str
                    if "." in line:
                        result.append(float(line))
                    else:
                        result.append(int(line))
                except ValueError:
                    result.append(line)
                i += 1
        return result

    # guided by expected structure
    result = []
    i = 0
    for exp in expected:
        if i >= len(raw_lines):
            break
        if isinstance(exp, list):
            # string list: count then strings
            try:
                n = int(raw_lines[i])
            except ValueError:
                result.append(raw_lines[i])
                i += 1
                continue
            i += 1
            items = []
            for _ in range(n):
                if i < len(raw_lines):
                    items.append(raw_lines[i])
                    i += 1
            result.append(items)
        elif exp == "empty":
            result.append("empty" if raw_lines[i] == "empty" else raw_lines[i])
            i += 1
        elif isinstance(exp, bool):
            result.append(raw_lines[i] == "true")
            i += 1
        elif isinstance(exp, float):
            result.append(float(raw_lines[i]))
            i += 1
        elif isinstance(exp, int) and not isinstance(exp, bool):
            try:
                result.append(int(raw_lines[i]))
            except ValueError:
                result.append(float(raw_lines[i]))
            i += 1
        elif isinstance(exp, str):
            result.append(raw_lines[i])
            i += 1
        else:
            result.append(raw_lines[i])
            i += 1
    return result


def decode_bool(stdout: str) -> bool:
    return stdout.strip().lower() == "true"


def decode_scalar(stdout: str):
    t = stdout.strip()
    if t == "true":
        return True
    if t == "false":
        return False
    if t == "null" or t == "empty":
        return None
    if "." in t:
        return float(t)
    return int(t)


def decode_list_i64(stdout: str) -> list:
    text = stdout.strip()
    if text == "" or text == "null":
        return [] if text != "null" else None  # type: ignore
    # first try space-separated single line / multi
    if text == "null":
        return None  # type: ignore
    parts = text.replace("\n", " ").split()
    if parts and parts[0] == "null":
        return None  # type: ignore
    return [int(x) for x in parts]


def decode_optional_list(stdout: str):
    text = stdout.strip()
    if text == "null" or text == "":
        # empty list vs null: null is explicit
        if text == "null":
            return None
        return []
    parts = text.replace("\n", " ").split()
    return [int(x) for x in parts]


def decode_pairs(stdout: str) -> list:
    lines = [ln.strip() for ln in stdout.splitlines() if ln.strip() != ""]
    if not lines:
        return []
    # first line is count
    try:
        n = int(lines[0])
        rest = lines[1:]
        out = []
        for i in range(n):
            if i < len(rest):
                a, b = rest[i].split()
                out.append([int(a), int(b)])
        return out
    except ValueError:
        # fallback flat
        parts = stdout.split()
        out = []
        for i in range(0, len(parts) - 1, 2):
            out.append([int(parts[i]), int(parts[i + 1])])
        return out


def decode_list_of_lists(stdout: str) -> list:
    lines = [ln.strip() for ln in stdout.splitlines() if ln.strip() != ""]
    if not lines:
        return []
    n = int(lines[0])
    out = []
    idx = 1
    for _ in range(n):
        if idx >= len(lines):
            break
        parts = lines[idx].split()
        idx += 1
        m = int(parts[0])
        vals = [int(x) for x in parts[1 : 1 + m]]
        out.append(vals)
    return out


def decode_tree(stdout: str):
    tokens: list[str] = []
    for ln in stdout.splitlines():
        tokens.extend(ln.split())
    it = iter(tokens)

    def parse():
        try:
            flag = next(it)
        except StopIteration:
            return None
        if flag == "0":
            return None
        val = int(next(it))
        left = parse()
        right = parse()
        return {"value": val, "left": left, "right": right}

    return parse()


def decode_adj(stdout: str):
    text = stdout.strip()
    if text == "null":
        return None
    lines = [ln.strip() for ln in stdout.splitlines() if ln.strip() != ""]
    if not lines:
        return []
    n = int(lines[0])
    g = []
    for i in range(n):
        parts = lines[1 + i].split()
        m = int(parts[0])
        row = []
        p = 1
        for _ in range(m):
            to = int(parts[p])
            w = int(parts[p + 1])
            row.append({"to": to, "weight": w})
            p += 2
        g.append(row)
    return g


def decode_maze_path(stdout: str) -> list:
    lines = [ln.strip() for ln in stdout.splitlines() if ln.strip() != ""]
    if not lines:
        return []
    n = int(lines[0])
    out = []
    for i in range(n):
        x, y = lines[1 + i].split()
        out.append({"x": int(x), "y": int(y)})
    return out


def normalize_out(shape: str, expected, stdout: str):
    if shape == "list_inplace":
        actual = decode_list_inplace(stdout)
        exp = expected if isinstance(expected, dict) else {"arr": expected}
        return exp, actual
    if shape == "commands":
        exp = list(expected)
        actual = decode_commands(stdout, exp)
        return exp, actual
    if shape in ("search", "tree_find", "tree_eq", "cycle"):
        return bool(expected), decode_bool(stdout)
    if shape in ("list_scalar", "list_k_scalar", "coin", "knapsack", "str2_scalar"):
        return expected, decode_scalar(stdout)
    if shape in ("list_list", "list_k_list", "topo"):
        return list(expected), decode_list_i64(stdout)
    if shape == "pairs":
        return [list(p) for p in expected], decode_pairs(stdout)
    if shape in ("list_lists", "combos"):
        return [list(row) for row in expected], decode_list_of_lists(stdout)
    if shape == "tree_list":
        return list(expected), decode_list_i64(stdout)
    if shape == "tree_map":
        return expected, decode_tree(stdout)
    if shape in ("graph_path", "matrix_path"):
        exp = None if expected is None else list(expected)
        return exp, decode_optional_list(stdout)
    if shape == "graph_mst":
        return expected, decode_adj(stdout)
    if shape == "maze":
        return expected, decode_maze_path(stdout)
    raise ValueError(f"unknown shape: {shape}")


def _norm_lists_of_lists(a):
    """Order of rows is free; order inside each row is significant (perms)."""
    rows = [tuple(row) for row in a]
    rows.sort()
    return rows


def _norm_adj(g):
    if g is None:
        return None
    out = []
    for row in g:
        edges = sorted(((e["to"], e["weight"]) for e in row))
        out.append(edges)
    return out


def _norm_tree(t):
    if t is None:
        return None
    return (
        int(t["value"]),
        _norm_tree(t.get("left")),
        _norm_tree(t.get("right")),
    )


def values_equal(a, b, shape: str | None = None) -> bool:
    if shape in ("list_lists", "combos"):
        if not isinstance(a, list) or not isinstance(b, list):
            return False
        return _norm_lists_of_lists(a) == _norm_lists_of_lists(b)
    if shape == "graph_mst":
        return _norm_adj(a) == _norm_adj(b)
    if shape == "tree_map":
        return _norm_tree(a) == _norm_tree(b)
    if shape == "commands":
        # element-wise with float tolerance and sorted str lists
        if not isinstance(a, list) or not isinstance(b, list):
            return False
        if len(a) != len(b):
            return False
        for x, y in zip(a, b):
            if isinstance(x, list) and isinstance(y, list):
                if sorted(x) != sorted(y):
                    return False
            elif isinstance(x, float) or isinstance(y, float):
                try:
                    if abs(float(x) - float(y)) > 1e-6:
                        return False
                except (TypeError, ValueError):
                    return False
            elif not values_equal(x, y):
                return False
        return True

    if type(a) != type(b) and not (isinstance(a, (int, float)) and isinstance(b, (int, float))):
        # allow int/float
        pass
    if isinstance(a, dict) and isinstance(b, dict):
        if a.keys() != b.keys():
            return False
        return all(values_equal(a[k], b[k]) for k in a)
    if isinstance(a, list) and isinstance(b, list):
        if len(a) != len(b):
            return False
        return all(values_equal(x, y) for x, y in zip(a, b))
    if isinstance(a, bool) or isinstance(b, bool):
        return a is b or a == b
    if isinstance(a, float) or isinstance(b, float):
        try:
            return abs(float(a) - float(b)) <= 1e-6
        except (TypeError, ValueError):
            return False
    if isinstance(a, (int, float)) and isinstance(b, (int, float)):
        return int(a) == int(b) if float(a) == int(a) and float(b) == int(b) else a == b
    return a == b


def lang_config(lang: str) -> tuple[str, str]:
    match lang:
        case "cpp":
            return ("//", ".cpp")
        case _:
            raise ValueError(f"unknown language: {lang}")


def _comment_block(kata: dict) -> str:
    lines = ["/*", f" * {kata.get('title', kata.get('id', ''))}", " *"]
    desc = (kata.get("description") or "").rstrip()
    if desc:
        for line in desc.splitlines():
            lines.append(f" * {line}" if line.strip() else " *")
        lines.append(" *")
    iface = (kata.get("interface") or "").rstrip()
    if iface:
        lines.append(" * interface:")
        for part in iface.splitlines():
            stripped = part.strip()
            lines.append(f" *   {stripped}" if stripped else " *")
    example = (kata.get("example") or "").rstrip()
    if example:
        lines.append(" *")
        lines.append(" * example:")
        for part in example.splitlines():
            lines.append(f" *   {part}" if part.strip() else " *")
    lines.append(" */")
    return "\n".join(lines)


_STUB_BODIES = {
    "list_inplace": """
#include "kata.hpp"

static void solve(std::vector<std::int64_t>& a) {
}

int main() {
    return kata::run_list_inplace(solve);
}
""",
    "commands": """
#include "kata.hpp"

static std::vector<kata::Reply> solve(const std::vector<std::int64_t>& init,
                                      const std::vector<kata::Op>& ops) {
    std::vector<kata::Reply> out;
    (void)init;
    for (const auto& op : ops) {
        (void)op;
    }
    return out;
}

int main() {
    return kata::run_commands(solve);
}
""",
    "search": """
#include "kata.hpp"

static bool solve(const std::vector<std::int64_t>& a, std::int64_t needle) {
    (void)a; (void)needle;
    return false;
}

int main() {
    return kata::run_search(solve);
}
""",
    "list_scalar": """
#include "kata.hpp"

static std::int64_t solve(const std::vector<std::int64_t>& a) {
    (void)a;
    return 0;
}

int main() {
    return kata::run_list_scalar(solve);
}
""",
    "list_k_scalar": """
#include "kata.hpp"

static std::int64_t solve(const std::vector<std::int64_t>& a, std::int64_t k) {
    (void)a; (void)k;
    return 0;
}

int main() {
    return kata::run_list_k_scalar(solve);
}
""",
    "list_list": """
#include "kata.hpp"

static std::vector<std::int64_t> solve(std::vector<std::int64_t> a) {
    return a;
}

int main() {
    return kata::run_list_list(solve);
}
""",
    "list_k_list": """
#include "kata.hpp"

static std::vector<std::int64_t> solve(std::vector<std::int64_t> a, std::int64_t k) {
    (void)k;
    return a;
}

int main() {
    return kata::run_list_k_list(solve);
}
""",
    "pairs": """
#include "kata.hpp"

static std::vector<std::pair<std::int64_t, std::int64_t>>
solve(std::vector<std::pair<std::int64_t, std::int64_t>> a) {
    return a;
}

int main() {
    return kata::run_pairs(solve);
}
""",
    "coin": """
#include "kata.hpp"

static std::int64_t solve(std::int64_t amount, const std::vector<std::int64_t>& coins) {
    (void)amount; (void)coins;
    return 0;
}

int main() {
    return kata::run_coin(solve);
}
""",
    "knapsack": """
#include "kata.hpp"

static std::int64_t solve(std::int64_t capacity,
                          const std::vector<std::int64_t>& weights,
                          const std::vector<std::int64_t>& values) {
    (void)capacity; (void)weights; (void)values;
    return 0;
}

int main() {
    return kata::run_knapsack(solve);
}
""",
    "str2_scalar": """
#include "kata.hpp"
#include <string>

static std::int64_t solve(const std::string& a, const std::string& b) {
    (void)a; (void)b;
    return 0;
}

int main() {
    return kata::run_str2_scalar(solve);
}
""",
    "tree_list": """
#include "kata.hpp"

static std::vector<std::int64_t> solve(kata::Node* root) {
    (void)root;
    return {};
}

int main() {
    return kata::run_tree_list(solve);
}
""",
    "tree_find": """
#include "kata.hpp"

static bool solve(kata::Node* root, std::int64_t needle) {
    (void)root; (void)needle;
    return false;
}

int main() {
    return kata::run_tree_find(solve);
}
""",
    "tree_eq": """
#include "kata.hpp"

static bool solve(kata::Node* a, kata::Node* b) {
    (void)a; (void)b;
    return false;
}

int main() {
    return kata::run_tree_eq(solve);
}
""",
    "tree_map": """
#include "kata.hpp"

static kata::Node* solve(kata::Node* root) {
    return root;
}

int main() {
    return kata::run_tree_map(solve);
}
""",
    "graph_path": """
#include "kata.hpp"
#include <optional>

static std::optional<std::vector<std::int64_t>>
solve(const kata::AdjList& g, int source, int sink) {
    (void)g; (void)source; (void)sink;
    return std::nullopt;
}

int main() {
    return kata::run_graph_path(solve);
}
""",
    "matrix_path": """
#include "kata.hpp"
#include <optional>

static std::optional<std::vector<std::int64_t>>
solve(const kata::Matrix& g, int source, int sink) {
    (void)g; (void)source; (void)sink;
    return std::nullopt;
}

int main() {
    return kata::run_matrix_path(solve);
}
""",
    "graph_mst": """
#include "kata.hpp"
#include <optional>

static std::optional<kata::AdjList> solve(const kata::AdjList& g) {
    (void)g;
    return std::nullopt;
}

int main() {
    return kata::run_graph_mst(solve);
}
""",
    "topo": """
#include "kata.hpp"

static std::vector<std::int64_t>
solve(int n, const std::vector<std::pair<std::int64_t, std::int64_t>>& edges) {
    (void)n; (void)edges;
    return {};
}

int main() {
    return kata::run_topo(solve);
}
""",
    "list_lists": """
#include "kata.hpp"

static std::vector<std::vector<std::int64_t>> solve(std::vector<std::int64_t> a) {
    (void)a;
    return {};
}

int main() {
    return kata::run_list_lists(solve);
}
""",
    "combos": """
#include "kata.hpp"

static std::vector<std::vector<std::int64_t>> solve(std::int64_t n, std::int64_t k) {
    (void)n; (void)k;
    return {};
}

int main() {
    return kata::run_combos(solve);
}
""",
    "maze": """
#include "kata.hpp"
#include <string>

static std::vector<kata::Point>
solve(const std::vector<std::string>& maze, const std::string& wall,
      kata::Point start, kata::Point end) {
    (void)maze; (void)wall; (void)start; (void)end;
    return {};
}

int main() {
    return kata::run_maze(solve);
}
""",
    "cycle": """
#include "kata.hpp"

static bool solve(kata::ListNode* head) {
    (void)head;
    return false;
}

int main() {
    return kata::run_cycle(solve);
}
""",
}


def render_cpp_stub(kata: dict) -> str:
    shape = kata.get("shape", "")
    body = _STUB_BODIES.get(shape)
    if not body:
        raise ValueError(f"no stub for shape: {shape}")
    return _comment_block(kata) + "\n" + body.lstrip("\n")


def render_stub(kata: dict, lang: str = "cpp") -> str:
    if lang == "cpp":
        return render_cpp_stub(kata)
    raise ValueError(f"unknown language: {lang}")
