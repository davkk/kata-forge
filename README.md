# kata-forge

C++ DSA practice forge. Compile-time assertions replace wire formats and I/O piping.

### Setup

Python 3 (stdlib only — no pip) and a C++17 compiler (`c++` / `g++` / `clang++`).

### Workflow

```
./kf pick          → pick 4 katas via FSRS, write to .active.json
./kf pick 6        → pick 6 katas
./kf generate      → create sessions/sessionN/ with stubs
# edit *.cpp stubs in sessions/sessionN/
./kf test          → compile & run test harnesses
./kf grade         → rate each kata from latest session (1=again .. 4=easy)
```

### Commands

| Command | Description |
|---------|-------------|
| `./kf list` | List all catalog katas |
| `./kf pick [N]` | FSRS picks N katas (default 4) into `.active.json` |
| `./kf pick --katas a,b` | Explicitly select katas |
| `./kf pick --random [N]` | Randomly pick N katas |
| `./kf generate` | Create `sessions/sessionN/` with stubs |
| `./kf test [session]` | Compile & run tests for a session (default: latest) |
| `./kf grade` | Interactive rating for latest session |
| `./kf grade --rate <kata> <grade>` | Rate a single kata |
| `./kf grade --state` | Show FSRS state for all reviewed katas |

### FSRS Scheduling

Default `./kf pick` uses **FSRS-5** spaced repetition. It shuffles all katas then sorts by **ascending retrievability** (lowest recall probability first), so you practice what you're about to forget.

Katas reviewed within the last 3 days get R=1.0 and sort to the end.

### Rating

After `./kf test`, run `./kf grade` to rate each kata:

| Grade | Meaning |
|-------|---------|
| 1 | Again (forgot) |
| 2 | Hard |
| 3 | Good |
| 4 | Easy |

Ratings update the FSRS card state (difficulty, stability, lapses) in `.fsrs.json`.

### FSRS State (`./kf grade --state`)

| Column | Meaning |
|--------|---------|
| **D** | Difficulty (1–10, higher = harder) |
| **S** | Stability in days (half-life analog) |
| **R** | Retrievability — estimated probability (0–1) you'd recall today |
| revs | Number of reviews |
| lapses | Number of times graded "again" |

### Layout

```
kf              single entrypoint
cli/            subcommand scripts
kflib/          Python library (kf_common, fsrs)
catalog/        one JSON per kata (meta, interface, tests)
sessions/       your practice sessions (sessionN/<id>.cpp)
goldens/        teaching write-ups
tests/          test harnesses
include/        C++ headers
.fsrs.json      FSRS card state
.active.json    current pick list
```

### Adding a kata

1. `catalog/<id>.json` — shape + named tests
2. `goldens/<id>.md` — teaching tutorial
3. If using a new shape, add its signature + harness logic in `kflib/kf_common.py`

### Stubs

Each stub includes `<bits/stdc++.h>` + `using namespace std;` + type definitions + function signature. No `main()`, no framework header. Fill in the function body.
