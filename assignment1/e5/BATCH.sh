#cc -o2 -o matrix_multiply.out matrix_multiply.c
#perf stat -e instructions,cycles,L1-dcache-load-misses,L1-dcache-loads ./matrix_multiply.out

cc -o2 -o transpose.out transpose.c
perf stat -e instructions,cycles,L1-dcache-load-misses,L1-dcache-loads ./transpose.out


