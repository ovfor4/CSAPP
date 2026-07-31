#!/usr/bin/env python3

"""
NOTA BENE
THIS IS FULLY DONE BY AI
DO NOT ASK ME WHY SOMEWHAT IS SOMEWHAT
THIS IS USED SOLELY FOR ILLUSTRATION AND EXPLANATION RATHER THAN ACTUAL COURSE WORK
THIS IS NOT CHEATING

Cache Lab 61x67: isolate the effect of the i2/j2 loop order.

This script models the cache used by the classic CS:APP Cache Lab:
    s = 5  -> 32 sets
    E = 1  -> direct mapped
    b = 5  -> 32-byte cache line = 8 int values

It compares two access orders for an 8x8 software tile:

    j2-i2: A is read vertically, B is written horizontally
    i2-j2: A is read horizontally, B is written vertically

The decisive control experiment is run in two modes:

    continuous:
        The cache is cold only at the start of the whole transpose.
        Cache state is preserved across software-tile boundaries.

    reset-per-tile:
        The cache is cleared before every software tile.
        This removes all cross-tile reuse while preserving the exact
        access order inside each tile.

For each order, the cross-tile benefit is:

    misses(reset-per-tile) - misses(continuous)

Default A/B addresses model the common Cache Lab harness where A and B are
separate 256x256 static int arrays. Their distance is 256*256*4 bytes, an
integer multiple of the 1 KiB cache capacity, so A and B have the same cache
set alignment.
"""

from __future__ import annotations

import argparse
from dataclasses import dataclass
from typing import Iterable, Iterator, Literal

Order = Literal["j2-i2", "i2-j2"]


@dataclass(frozen=True)
class Config:
    m: int = 61                 # A columns, B rows
    n: int = 67                 # A rows, B columns
    tile: int = 8
    sets: int = 32
    line_bytes: int = 32
    int_bytes: int = 4
    a_base: int = 0
    b_base: int = 256 * 256 * 4

    def validate(self) -> None:
        values = {
            "m": self.m,
            "n": self.n,
            "tile": self.tile,
            "sets": self.sets,
            "line_bytes": self.line_bytes,
            "int_bytes": self.int_bytes,
        }
        for name, value in values.items():
            if value <= 0:
                raise ValueError(f"{name} must be positive, got {value}")
        if self.line_bytes % self.int_bytes != 0:
            raise ValueError("line_bytes must be divisible by int_bytes")


@dataclass
class Stats:
    a_hits: int = 0
    a_misses: int = 0
    b_hits: int = 0
    b_misses: int = 0

    @property
    def total_hits(self) -> int:
        return self.a_hits + self.b_hits

    @property
    def total_misses(self) -> int:
        return self.a_misses + self.b_misses


class DirectMappedCache:
    """Minimal direct-mapped cache: one tag per set."""

    def __init__(self, sets: int, line_bytes: int) -> None:
        self.sets = sets
        self.line_bytes = line_bytes
        self.tags: list[int | None] = [None] * sets

    def clear(self) -> None:
        self.tags = [None] * self.sets

    def access(self, address: int) -> bool:
        """Return True on hit and False on miss; install line on every access."""
        block_number = address // self.line_bytes
        set_index = block_number % self.sets
        tag = block_number // self.sets

        hit = self.tags[set_index] == tag
        self.tags[set_index] = tag
        return hit


def address_a(cfg: Config, row: int, col: int) -> int:
    return cfg.a_base + cfg.int_bytes * (row * cfg.m + col)


def address_b(cfg: Config, row: int, col: int) -> int:
    return cfg.b_base + cfg.int_bytes * (row * cfg.n + col)


def tile_elements(cfg: Config, i: int, j: int, order: Order) -> Iterator[tuple[int, int]]:
    """Yield A[row][col] coordinates in the exact loop order."""
    if order == "j2-i2":
        for j2 in range(cfg.tile):
            for i2 in range(cfg.tile):
                row = i + i2
                col = j + j2
                if row < cfg.n and col < cfg.m:
                    yield row, col
    elif order == "i2-j2":
        for i2 in range(cfg.tile):
            for j2 in range(cfg.tile):
                row = i + i2
                col = j + j2
                if row < cfg.n and col < cfg.m:
                    yield row, col
    else:
        raise ValueError(f"unsupported order: {order}")


def simulate(cfg: Config, order: Order, reset_per_tile: bool) -> Stats:
    cache = DirectMappedCache(cfg.sets, cfg.line_bytes)
    stats = Stats()

    for i in range(0, cfg.n, cfg.tile):
        for j in range(0, cfg.m, cfg.tile):
            if reset_per_tile:
                cache.clear()

            for a_row, a_col in tile_elements(cfg, i, j, order):
                # Original code performs one A load followed immediately by one B store.
                if cache.access(address_a(cfg, a_row, a_col)):
                    stats.a_hits += 1
                else:
                    stats.a_misses += 1

                # Transpose: A[a_row][a_col] -> B[a_col][a_row]
                if cache.access(address_b(cfg, a_col, a_row)):
                    stats.b_hits += 1
                else:
                    stats.b_misses += 1

    return stats


def row(label: str, stats: Stats) -> str:
    return (
        f"{label:<22}"
        f"{stats.a_misses:>11}"
        f"{stats.b_misses:>11}"
        f"{stats.total_misses:>14}"
    )


def print_report(cfg: Config) -> None:
    results: dict[tuple[Order, str], Stats] = {}
    for order in ("j2-i2", "i2-j2"):
        results[(order, "continuous")] = simulate(cfg, order, reset_per_tile=False)
        results[(order, "reset")] = simulate(cfg, order, reset_per_tile=True)

    print("Cache Lab loop-order control experiment")
    print("=" * 69)
    print(
        f"matrix A={cfg.n}x{cfg.m}, B={cfg.m}x{cfg.n}, "
        f"tile={cfg.tile}x{cfg.tile}"
    )
    print(
        f"cache: {cfg.sets} sets, direct mapped, "
        f"{cfg.line_bytes} B/line, {cfg.line_bytes // cfg.int_bytes} ints/line"
    )
    print(f"A base = {cfg.a_base:#x}, B base = {cfg.b_base:#x}")
    print()

    print(f"{'case':<22}{'A misses':>11}{'B misses':>11}{'total misses':>14}")
    print("-" * 58)
    for order in ("j2-i2", "i2-j2"):
        print(row(f"{order} continuous", results[(order, "continuous")]))
        print(row(f"{order} reset/tile", results[(order, "reset")]))
        print()

    print("Cross-tile benefit = reset-per-tile misses - continuous misses")
    print(f"{'order':<12}{'A benefit':>12}{'B benefit':>12}{'total':>12}")
    print("-" * 48)

    benefits: dict[Order, tuple[int, int, int]] = {}
    for order in ("j2-i2", "i2-j2"):
        continuous = results[(order, "continuous")]
        reset = results[(order, "reset")]
        a_benefit = reset.a_misses - continuous.a_misses
        b_benefit = reset.b_misses - continuous.b_misses
        total_benefit = reset.total_misses - continuous.total_misses
        benefits[order] = (a_benefit, b_benefit, total_benefit)
        print(f"{order:<12}{a_benefit:>12}{b_benefit:>12}{total_benefit:>12}")

    reset_gap = (
        results[("j2-i2", "reset")].total_misses
        - results[("i2-j2", "reset")].total_misses
    )
    continuous_advantage = (
        results[("i2-j2", "continuous")].total_misses
        - results[("j2-i2", "continuous")].total_misses
    )
    extra_a_cross_tile = benefits["j2-i2"][0] - benefits["i2-j2"][0]
    extra_b_cross_tile = benefits["j2-i2"][1] - benefits["i2-j2"][1]

    print()
    print("Derived comparison")
    print("-" * 69)
    print(f"Per-tile-only gap (j2-i2 - i2-j2): {reset_gap:+d} misses")
    print(
        "Extra cross-tile benefit of j2-i2: "
        f"A={extra_a_cross_tile:+d}, B={extra_b_cross_tile:+d}"
    )
    print(
        "Final continuous-cache advantage of j2-i2: "
        f"{continuous_advantage} fewer misses"
    )

    # These equalities are the point of the experiment for the default setup.
    if cfg == Config():
        assert results[("j2-i2", "continuous")].a_misses == 847
        assert results[("j2-i2", "continuous")].b_misses == 1081
        assert results[("j2-i2", "reset")].a_misses == 1223
        assert results[("j2-i2", "reset")].b_misses == 1081
        assert results[("i2-j2", "continuous")].a_misses == 877
        assert results[("i2-j2", "continuous")].b_misses == 1238
        assert results[("i2-j2", "reset")].a_misses == 1062
        assert results[("i2-j2", "reset")].b_misses == 1238
        assert continuous_advantage == 187
        assert extra_a_cross_tile == 191
        assert extra_b_cross_tile == 0


def parse_int(text: str) -> int:
    """Accept decimal or 0x-prefixed integers."""
    return int(text, 0)


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Compare i2/j2 loop orders with and without cross-tile cache state."
    )
    parser.add_argument("--m", type=int, default=61)
    parser.add_argument("--n", type=int, default=67)
    parser.add_argument("--tile", type=int, default=8)
    parser.add_argument("--sets", type=int, default=32)
    parser.add_argument("--line-bytes", type=int, default=32)
    parser.add_argument("--int-bytes", type=int, default=4)
    parser.add_argument("--a-base", type=parse_int, default=0)
    parser.add_argument("--b-base", type=parse_int, default=256 * 256 * 4)
    return parser


def main() -> None:
    args = build_parser().parse_args()
    cfg = Config(
        m=args.m,
        n=args.n,
        tile=args.tile,
        sets=args.sets,
        line_bytes=args.line_bytes,
        int_bytes=args.int_bytes,
        a_base=args.a_base,
        b_base=args.b_base,
    )
    cfg.validate()
    print_report(cfg)


if __name__ == "__main__":
    main()
