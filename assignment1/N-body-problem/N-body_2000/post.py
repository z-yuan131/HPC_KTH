import numpy as np
import locale
locale.setlocale( locale.LC_ALL, 'en_US.UTF-8' )

n = 50

ins = 0
inspc = 0
c = 0
L1_miss = 0
ratio = 0
L1 = 0

b = np.zeros(n)

for i in range(n):
	file = open('./out/out_'+str(i+1)+'.txt')
	for j in range(11):
		a = next(file)

		if j == 10:
			print(a.split(' ')[6])
			b[i] = np.array(a.split(' ')[6],np.float64)

file = open('./out/out_'+str(n)+'.txt')
for i in range(11):
	print(next(file))



print('average elasps time\n')
print(np.sum(b)/n)
print('max = '+str(max(b)))
print('min = '+str(min(b)))
