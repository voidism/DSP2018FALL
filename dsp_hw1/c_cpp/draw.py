import matplotlib.pyplot as plt
import numpy as np
f = open("trajectory.txt").read().split('\n')[:-1]
l = [[int(x.split(':')[0]), float(x.split(':')[1])] for x in f]
n = np.array(l)
plt.plot(n[:, 0], n[:, 1])
plt.ylabel('accuracy')
plt.xlabel('iteration')
plt.title('analysis of accuracy v.s. iteration')
plt.savefig('ana.png')
