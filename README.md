# kata-forge

C++ DSA kata practice.

### Setup

Python 3 (stdlib only) and a C++17 compiler (`c++` / `g++` / `clang++`).

### Workflow

```
kf pick          -> pick 4 katas via FSRS, write to .active.json
kf pick 6        -> pick 6 katas
kf generate      -> create sessions/sessionN/ with stubs
kf golden <kata> -> render the reference tutorial for a kata

# edit *.cpp stubs in sessions/sessionN/
kf test          -> compile & run test harnesses
kf grade         -> rate each kata from latest session (1=again .. 4=easy)
```

### FSRS Scheduling

Default `kf pick` uses **FSRS-5** spaced repetition. It shuffles all katas then sorts by **ascending retrievability** (lowest recall probability first), so you practice what you're about to forget.

Katas reviewed within the last 3 days get R=1.0 and sort to the end.

### Rating

After `kf test`, run `kf grade` to rate each kata:

| Grade | Meaning |
|-------|---------|
| 1 | Again (forgot) |
| 2 | Hard |
| 3 | Good |
| 4 | Easy |

Ratings update the FSRS card state (difficulty, stability, lapses).

### FSRS State (`kf grade --state`)

| Column | Meaning |
|--------|---------|
| **D** | Difficulty (1–10, higher = harder) |
| **S** | Stability in days (half-life analog) |
| **R** | Retrievability — estimated probability (0–1) you'd recall today |
| revs | Number of reviews |
| lapses | Number of times graded "again" |

### Adding a kata

1. `catalog/<id>.json` — meta + `cpp` interface block
2. `goldens/<id>.md` — teaching tutorial
3. `tests/<id>.test.cpp` — test harness

### Stubs

Each stub includes `<bits/stdc++.h>` + `using namespace std;` + type definitions + function signature. No `main()`, no framework header. Fill in the function body.
