#!/bin/bash
rm stream_modified_loop_output.txt
#export OMP_NUM_THREADS=4
#cc -fopenmp stream_modified.c -o stream_modified.out
#np=(1 2 4 8 12 16 20 24 28 32)
for np in 1 2 4 8 12 16 20 24 28 32
	do
	export OMP_NUM_THREADS=$np
	cc -fopenmp stream_modified.c -o stream_modified.out
	for i in {1..5}
		do
   		echo "$i times for Threads= $np"
   		echo "$i times for Threads= $np" >>stream_modified_loop_output.txt
   		./stream_modified.out >>stream_modified_loop_output.txt      
   		echo " " >> stream_modified_loop_output.txt
		done
done
cat stream_modified_loop_output.txt


