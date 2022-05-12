#!/bin/bash -l
# The -l above is required to get the full environment with modules

# The name of the script is myjob
#SBATCH -J myjob
# Only 1 hour wall-clock time will be given to this job
#SBATCH -t 00:10:00
#SBATCH -A edu22.DD2356
# Number of nodes
#SBATCH -p main
#SBATCH --ntasks-per-node=2
#SBATCH --nodes=1

for i in {1...5}
do
	echo "round $i" >> on_node.out
	srun -n 2 ./ping_pong >> on_node.out
done
