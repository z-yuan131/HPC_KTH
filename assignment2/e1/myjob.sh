#!/bin/bash -l
# The -l above is required to get the full environment with modules

# The name of the script is myjob
#SBATCH -J myjob
# Only 1 hour wall-clock time will be given to this job
#SBATCH -t 00:01:00
#SBATCH -A edu22.DD2356
# Number of nodes
#SBATCH -p shared
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=16
#SBATCH --nodes=1
#SBATCH -e error_file.e

export OMP_NUM_THREADS=8
cc -fopenmp -o helloworld_para_dardel.out helloworld_para_dardel.c 
srun -c 8 ./helloworld_para_dardel.out > hello_output
cat hello_output
