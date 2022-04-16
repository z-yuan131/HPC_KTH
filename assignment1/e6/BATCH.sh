#salloc --nodes=1 -t 02:00:00 -A edu22.DD2356 -p shared

#module swap PrgEnv-cray PrgEnv-gnu
#perf stat -e instructions,cycles,L1-dcache-load-misses,L1-dcache-loads ./matrix_multiply.out
gcc -O3 -fopt-info-vec-all  matrix_multiply.c >> report.txt
