import numpy as np

b = 0
for i in range(100):
	file = open('./out/out_'+str(i+1)+'.txt')
	for j in range(11):
		a = next(file)
		if j == 10:
			#print(a.split(' ')[7])
			b += np.array(a.split(' ')[7],np.float64)

print(b/100)
