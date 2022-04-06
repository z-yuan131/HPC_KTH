#!/bin/bash
rm benchmark.out

cc -o2 -o benchmark_op.out benchmark.c
#cc -S -fverbose-asm -O2 benchmark.c
#cc -o benchmark.out benchmark.c

rm output.txt
for i in {1..1000}
do
   echo "Welcome $i times"
   ./benchmark_op.out >> output.txt  
   #./benchmark.s >> output.txt	   
   echo " " >> output.txt 
done

python3 postpro.py 1000
