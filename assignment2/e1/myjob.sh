cc -fopenmp -o helloworld_para_dardel.out helloworld_para_dardel.c 
srun -n 1 ./helloworld_para_dardel.out > hello_output
cat hello_output
