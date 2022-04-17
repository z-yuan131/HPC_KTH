cc -openmp DFTW_1.c
#cc -openmp DFTW_2.c

for i in {1,2,4,8,12,16,20,24,28,32}
do
	export OMP_NUM_THREADS=$i
	#srun -n 1 ./a.out > out_$i.txt
	./a.out > out_$i.txt
done

rm -r outfile
mkdir outfile
mv *.txt outfile
#python post.py
