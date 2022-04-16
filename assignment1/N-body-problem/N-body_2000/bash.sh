
rm a.out
#cc -o2 -o a.out N-body_simple.c -lm
cc -o2 -o a.out N-body_reduce.c -lm
#./a.out 0.05 1

for i in {1..50}
do 
	perf stat -o out_$i.txt -e instructions,cycles,L1-dcache-load-misses,L1-dcache-loads ./a.out 0.05 100
done

mkdir out
mv *.txt out

python3 post.py >> result.txt
