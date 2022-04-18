cc -openmp shwater2d_par.c

for i in {1..16}
do
	export OMP_NUM_THREADS=$i
	#srun -n 1 ./a.out > out_$i.txt
	./a.out > out_$i.txt
done

rm -r outfile
mkdir outfile
mv *.txt outfile
python post.py
