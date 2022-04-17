cc -openmp sum.c
for i in {1..10}
do
	srun -n 1 ./a.out > out_$i.txt
done

rm -r outfile
mkdir outfile
mv *.txt outfile

python post.py
