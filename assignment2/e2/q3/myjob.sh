#!/bin/bash
#rm stream_guided_output.txt
export OMP_NUM_THREADS=32
cc -fopenmp stream.c -o stream.out
for i in {1..5}
	do
	echo i=$i
   	./stream.out >>stream_output_modified.txt      
done
cat stream_guided2048_output_modified.txt




