# kata-forge — agent notes

Multi-language DSA practice forge. **v1 = C++ only.** Keep changes aligned with these choices.

## Core model

- **Catalog** `catalog/<id>.json` — one file per kata: meta, description, interface, named tests (`in`/`out`).
- **Shapes** — plain-text wire formats (no JSON on the wire). See `shapes.md`.
  - `list_inplace`, `commands`, `search`, `list_scalar`, `list_k_scalar`, …
  - trees / graphs / maze / cycle / pairs / coin / knapsack / strings / combos
- **Lib** `lib/<lang>/` — shape I/O helpers only (`run_*`). Not algo code.
- **Sessions** live in-repo: `sessions/sessionN/`. Pick list: `.active.json`.
- **Goldens** `goldens/<id>.md` — concise reference (intuition, code variants, where useful). No runnable golden `.cpp`.
- **CLIs** (repo root, Python 3 **stdlib only**): `list`, `pick`, `generate`, `test`.

## Generate / stubs

- `./generate` writes `sessions/sessionN/<id>.cpp`.
- Problem text goes in a short `/* ... */` block: title, how it works, interface, one example.
- No I/O docs, no run instructions, no golden paths in the stub comment.
- Empty `solve` + `main` wired to the right shape helper.

## Goldens (markdown)

- Teaching tutorials: intuition → main approach + code → alternatives → where it shows up → cousins & contrasts.
- Code in **C-style C++** with `using namespace std;` — raw pointers for linked structures, plain arrays/vectors, free functions. No templates, no classes with access specifiers, no TypeScript.
- Bullet points only; **no tables**, no numbered step-lists that merely paraphrase code.
- ~50–100 lines per file (≤120 for range queries; ≥40 for trivial structures). Every line justifies itself.
- No kata-wiring — never mention shapes, catalog, sessions, `./test`, I/O formats.
- Each golden must be self-contained: define the state (dp, recurrence, invariant) and teach why, not just how.

## Testing

- `./test` compiles session `*.cpp` with `-I lib/cpp`, feeds shape-encoded stdin, diffs stdout.
- Catalog structured data ↔ shape text lives in `kf_common.py` (must match `lib/cpp/kata.hpp`).
- Escape hatch: `./test --kata ID --cmd '...'`.

## Adding a kata

1. `catalog/<id>.json` with `shape` + named tests.
2. `goldens/<id>.md` reference write-up.
3. Stub skeleton path in `render_cpp_stub` if new shape.

## Adding a language later

1. `lib/<lang>/` mirroring the same helpers and wire rules.
2. Teach `./test` how to run that lang’s files.
3. Catalog / shapes / judge encode-decode stay unchanged.

## Do not

- Add pip dependencies / PyYAML.
- Put sessions or state under XDG.
- Reintroduce per-lang signature codegen or C ABI/ctypes.
- Use JSON as the solution wire format.
- Make stub comments or goldens verbose.
