"""FSRS-5 spaced repetition scheduler for kata-forge."""
from __future__ import annotations

import json
import math
from datetime import date, datetime, timezone
from pathlib import Path

W = [
    0.40255, 1.18385, 3.173, 15.69105,
    7.1949, 0.5345, 1.4604, 0.0046,
    1.54575, 0.1192, 1.01925, 1.9395,
    0.11, 0.29605, 2.2698, 0.2315,
    2.9898, 0.51655, 0.6621,
]

DECAY = -0.5
FACTOR = 19 / 81
FSRS_PATH = Path(__file__).resolve().parent / ".fsrs.json"


def _clamp(v: float, lo: float = 1.0, hi: float = 10.0) -> float:
    return max(lo, min(hi, v))


def retrievability(stability: float, days_since_review: int) -> float:
    if stability < 0.01:
        return 0.0
    t = max(0, days_since_review)
    return (1 + FACTOR * t / stability) ** DECAY


def _initial_stability(grade: int) -> float:
    return W[grade - 1]


def _initial_difficulty(grade: int) -> float:
    return W[4] - math.exp(W[5] * (grade - 1)) + 1


def _next_difficulty(d: float, grade: int) -> float:
    delta = -W[6] * (grade - 3)
    d_prime = d + delta * (10 - d) / 9
    d0_4 = _initial_difficulty(4)
    return _clamp(W[7] * d0_4 + (1 - W[7]) * d_prime)


def _stability_after_recall(d: float, s: float, r: float, grade: int) -> float:
    inc = W[8] * (11 - d) * (s ** -W[9]) * (math.exp(W[10] * (1 - r)) - 1)
    mult = 1.0
    if grade == 2:
        mult *= W[15]
    elif grade == 4:
        mult *= W[16]
    return s * (1 + inc * mult)


def _stability_after_forget(d: float, s: float, r: float) -> float:
    return W[11] * (d ** -W[12]) * ((s + 1) ** W[13] - 1) * math.exp(W[14] * (1 - r))


# --- state management ---------------------------------------------------------

def load_state() -> dict:
    try:
        return json.loads(FSRS_PATH.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError):
        return {}


def save_state(state: dict) -> None:
    FSRS_PATH.write_text(json.dumps(state, indent=2) + "\n", encoding="utf-8")


def get_card(state: dict, kid: str) -> dict:
    return state.setdefault(kid, {
        "difficulty": _initial_difficulty(3),
        "stability": 0.0,
        "last_review": None,
        "review_count": 0,
        "lapses": 0,
    })


def _stability_same_day(s: float, grade: int) -> float:
    return s * math.exp(W[17] * (grade - 3 + W[18]))


def review(kid: str, grade: int, today: date | None = None) -> dict:
    if today is None:
        today = datetime.now(timezone.utc).date()
    state = load_state()
    card = get_card(state, kid)
    d = card["difficulty"]
    s = card["stability"]
    rc = card["review_count"]
    lapses = card["lapses"]

    if rc == 0:
        s = _initial_stability(grade)
        d = _initial_difficulty(grade)
        if grade == 1:
            lapses += 1
    else:
        lr = card["last_review"]
        delta = (today - date.fromisoformat(lr)).days if lr else 0
        same_day = delta < 1
        if same_day and grade > 1:
            s = _stability_same_day(s, grade)
        elif grade == 1:
            r = retrievability(s, delta)
            s = _stability_after_forget(d, s, r)
            lapses += 1
        else:
            r = retrievability(s, delta)
            s = _stability_after_recall(d, s, r, grade)
        d = _next_difficulty(d, grade)

    card["difficulty"] = d
    card["stability"] = s
    card["last_review"] = today.isoformat()
    card["review_count"] = rc + 1
    card["lapses"] = lapses
    save_state(state)
    return card


def retrievability_of(kid: str, today: date | None = None, min_interval: int = 0) -> float:
    if today is None:
        today = datetime.now(timezone.utc).date()
    state = load_state()
    card = get_card(state, kid)
    if card["last_review"] is None:
        return 0.0
    delta = (today - date.fromisoformat(card["last_review"])).days
    if delta < min_interval:
        return 1.0
    return retrievability(card["stability"], delta)
