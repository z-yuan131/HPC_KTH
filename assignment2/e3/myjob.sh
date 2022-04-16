#!/bin/bash
rm sum_output.txt
for i in {1,2,4,8,16,20,24,28,32}
do
   export OMP_NUM_THREADS=$i
   cc -fopenmp sum.c  -o sum.out
   echo "$i Threads"
   echo "$i Threads" >> sum_output.txt
   ./sum.out >> sum_output.txt
   echo " " >> sum_output.txt
done


