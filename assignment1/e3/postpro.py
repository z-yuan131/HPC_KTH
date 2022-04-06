import numpy as np
import sys
a = np.loadtxt('output.txt')

print(min(a))
print(np.sum(a)/int(sys.argv[1]))
