import numpy as np
import sys
# a = np.loadtxt('output.txt')

# print(min(a))
# print(np.sum(a)/int(sys.argv[1]))

a = np.linspace(1,16,16,dtype='int')
print(a)

perf = np.zeros(len(a))

for i in range(len(a)):
	file = f'./outfile/out_{a[i]}.txt'
	f = open(file)
	#print(next(f).split(' ')[-2])
	perf[i] = next(f).split(' ')[-2]	

print(perf[0]/perf)
print(perf)
