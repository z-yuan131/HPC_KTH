import numpy as np
import sys
# a = np.loadtxt('output.txt')

# print(min(a))
# print(np.sum(a)/int(sys.argv[1]))

a = np.linspace(1,10,10,dtype="int")


perf = np.zeros(len(a))

for i in range(len(a)):
        file = f'./outfile/out_{a[i]}.txt'
        f = open(file)
        next(f)
        perf[i] = next(f).split(' ')[-2]

#print(perf[0]/perf)
#print(perf)

print('32 threads:',str(np.sum(perf)/len(a)), str(np.std(perf)))
