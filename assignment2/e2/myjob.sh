#!/bin/bash
rm stream_modified_output.txt
export OMP_NUM_THREADS=4
cc -fopenmp stream_modified.c -o stream_modified.out
for i in {1..5}
do
   echo "$i times"
   echo "$i times" >>stream_modified_output.txt
   ./stream_modified.out >>stream_modified_output.txt      
   echo " " >> stream_modified_output.txt
done
cat stream_modified_output.txt


