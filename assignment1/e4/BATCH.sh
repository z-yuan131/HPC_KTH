#salloc --nodes=1 -t 02:00:00 -A edu22.DD2356 -p shared

#cc -o2 -o matrix_multiply.out matrix_multiply.c
#perf stat -e instructions,cycles,L1-dcache-load-misses,L1-dcache-loads ./matrix_multiply.out

cc -o2 -o matrix_multiply_op.out matrix_multiply_op.c
perf stat -e instructions,cycles,L1-dcache-load-misses,L1-dcache-loads ./matrix_multiply_op.out

