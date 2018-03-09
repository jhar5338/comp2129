#!/usr/bin/env sh

echo "test: str-empty"
	./minesweeper < tests/str-empty.in | diff - tests/error.out && echo "passed!" || echo"failed"

echo "test: str-line"
	./minesweeper < tests/str-line.in | diff - tests/error.out && echo "passed!" || echo"failed"

echo "test: str-char"
	./minesweeper < tests/str-char.in | diff - tests/error.out && echo "passed!" || echo"failed"

echo "test: str-one-int"
	./minesweeper < tests/str-one-int.in | diff - tests/error.out && echo "passed!" || echo"failed"

echo "test: str-three-int"
	./minesweeper < tests/str-three-int.in | diff - tests/error.out && echo "passed!" || echo"failed"

echo "test: grid-big"
	./minesweeper < tests/grid-big.in | diff - tests/error.out && echo "passed!" || echo"failed"

echo "test: grid-small"
	./minesweeper < tests/grid-small.in | diff - tests/error.out && echo "passed!" || echo"failed"

echo "test: grid-wrong-char"
	./minesweeper < tests/grid-wrong-char.in | diff - tests/error.out && echo "passed!" || echo"failed"
echo "test: flag-first"
	./minesweeper < tests/flag-first.in | diff - tests/flag-first.out && echo "passed!" || echo"failed"

echo "test: uncover-first"
	./minesweeper < tests/uncover-first.in | diff - tests/uncover-first.out && echo "passed!" || echo"failed"

echo "test: flag-first"
	./minesweeper < tests/flag-bomb-then-uncover.in | diff - tests/flag-bomb-then-uncover.out && echo "passed!" || echo"failed"

