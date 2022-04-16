module swap PrgEnv-cray PrgEnv-gnu
salloc --nodes=1 -t 01:00:00 -A edu22.DD2356 -p shared
srun -n 1 ./name 
