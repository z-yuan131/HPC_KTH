import numpy as np
a = []
file = open('output.txt','r')
lines = file.readlines()

for i in lines:
	a.append(float(i.split(' ')[-2]))

print(a)

b = np.sum(np.array(a))/len(a)



f = open('result_mean.txt','a')
f.write(str(b)+',')
f.close()

#f = open('result_max.txt','a')
#f.write(str(max(a))+',')
#f.close()

#f = open('result_min.txt','a')
#f.write(str(min(a))+',')
#f.close()

f = open('error.txt','a')
f.write(str(np.std(np.array(a)))+',')
f.close()

file.close()
