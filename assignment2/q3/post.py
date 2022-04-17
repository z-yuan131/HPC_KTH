import numpy as np
import sys
# a = np.loadtxt('output.txt')

# print(min(a))
# print(np.sum(a)/int(sys.argv[1]))

a = np.linspace(1,10,10,dtype="int")


serial = np.zeros(len(a))
par1 = np.zeros(len(a))
par2 = np.zeros(len(a))
par3 = np.zeros(len(a))
par4 = np.zeros(len(a))

for i in range(len(a)):
	file = f'./outfile/out_{a[i]}.txt'
	f = open(file)
	serial[i] = next(f).split(' ')[-2]
	serial_result = next(f).split(' ')[-2]	
	par1[i] = next(f).split(' ')[-2]
	par1_resullt = next(f).split(' ')[-2]
	par2[i] = next(f).split(' ')[-2]
	par2_resullt = next(f).split(' ')[-2]
	par3[i] = next(f).split(' ')[-2]
	par3_resullt = next(f).split(' ')[-2]
	par4[i] = next(f).split(' ')[-2]
	par4_resullt = next(f).split(' ')[-2]

print('serial:',str(np.sum(serial)/len(a)), str(np.std(serial)), str(serial_result))
