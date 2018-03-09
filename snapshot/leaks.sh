# Compile your program with debugging symbols
clang -g -Wall -Werror -std=c11 snapshot.c -o snapshot

# Run your program against each input file
for t in tests/*.in
do
    # Check your program for memory leaks
    printf "%s\n" $(basename $t)
    valgrind ./snapshot < $t
done
