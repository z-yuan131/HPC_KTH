
rm a.out
#cc -o2 -o a.out N-body_simple.c -lm
cc -o2 -o a.out N-body_reduce.c -lm
./a.out 0.05 1
