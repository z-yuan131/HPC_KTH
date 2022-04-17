cc -openmp DFTW_1.c
for i in {1..10}
do
        export OMP_NUM_THREADS=32
	./a.out > out_$i.txt
done

rm -r outfile
mkdir outfile
mv *.txt outfile

python post.py
