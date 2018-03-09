#!/usr/bin/env sh

clang -g -Wall -Werror -std=gnu11 -fsanitize=address snapshot.c -o snapshot

printf "bye: "
	./snapshot < tests/bye.in | diff - tests/bye.out && echo "passed!" || echo "failed"

printf "caps: "
	./snapshot < tests/caps.in | diff - tests/bye.out && echo "passed!" || echo "failed"

printf "help: "
	./snapshot < tests/help.in | diff - tests/help.out && echo "passed!\n" || echo "failed\n"

printf "list-keys: "
	./snapshot < tests/list-keys.in | diff - tests/list-keys.out && echo "passed!" || echo "failed"

printf "list-entries: "
		./snapshot < tests/list-entries.in | diff - tests/list-entries.out && echo "passed!\n" || echo "failed\n"

printf "set-get: "
	./snapshot < tests/set-get.in | diff - tests/set-get.out && echo "passed!" || echo "failed"

printf "set-set: "
	./snapshot < tests/set-set.in | diff - tests/set-set.out && echo "passed!" || echo "failed"

printf "del: "
	./snapshot < tests/del.in | diff - tests/del.out && echo "passed!\n" || echo "failed\n"

printf "push: "
	./snapshot < tests/push.in | diff - tests/push.out && echo "passed!" || echo "failed"

printf "append: "
		./snapshot < tests/append.in | diff - tests/append.out && echo "passed!\n" || echo "failed\n"

printf "pick: "
	./snapshot < tests/pick.in | diff - tests/pick.out && echo "passed!" || echo "failed"

printf "pluck: "
	./snapshot < tests/pluck.in | diff - tests/pluck.out && echo "passed!" || echo "failed"

printf "pop: "
	./snapshot < tests/pop.in | diff - tests/pop.out && echo "passed!\n" || echo "failed\n"

printf "min: "
	./snapshot < tests/min.in | diff - tests/min.out && echo "passed!" || echo "failed"

printf "max: "
	./snapshot < tests/max.in | diff - tests/max.out && echo "passed!" || echo "failed"

printf "sum: "
	./snapshot < tests/sum.in | diff - tests/sum.out && echo "passed!" || echo "failed"

printf "len: "
	./snapshot < tests/len.in | diff - tests/len.out && echo "passed!" || echo "failed"
