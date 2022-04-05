import numpy as np
import locale
locale.setlocale( locale.LC_ALL, 'en_US.UTF-8' )

n = 100

ins = 0
inspc = 0
c = 0
L1_miss = 0
ratio = 0
L1 = 0

b = 0

for i in range(n):
	print(i)
	file = open('./out/out_'+str(i+1)+'.txt')
	for j in range(11):
		a = next(file)
		if j == 5:
			ins += locale.atof(a.split(' ')[4])
			inspc += locale.atof(a.split(' ')[26])
		if j == 6:
			c += locale.atof(a.split(' ')[5])
		#if j == 7:
			#print(a.split(' ')[9])
			#L1_miss += locale.atof(a.split(' ')[9])
			#ratio += locale.atof(a.split(' ')[22][0:4])
		if j == 8:
			L1 += locale.atof(a.split(' ')[5])
		if j == 10:
			#print(a.split(' ')[7])
			b += np.array(a.split(' ')[7],np.float64)
                        
                        
print('instructions:')
print(ins/n)
print('\n')

print('insc per cycle')
print(inspc/n)
print('\n')

print('cycle')
print(c/n)
print('\n')

print('L1 cache miss')
print(L1_miss/n)
print('\n')

print('L1 cache miss ratio')
print(ratio/n)
print('\n')

print('L1 cache load')
print(L1/n)
print('\n')

print('elasps time')
print(b/n)

