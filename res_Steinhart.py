import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

fs = open("outputDat.txt", 'r')
data = [[float(i) for i in j[1:-1].split(',')] for j in fs.read().split('\n')[:-1] if float(j[1:-1].split(',')[0]) > 0]
fs.close()

temp = np.array([i[0] + 298.15 for i in data])
res = np.array([i[1] for i in data])

def steinhart(R, a, b, c):
    return 1 / (a + b*np.log(R) + c * (np.log(R)) ** 3)

popt, pcov = curve_fit(steinhart, res, temp)

print(popt)

plt.plot(res,temp - 298.15, 'bo', ms=0.5)
plt.plot(np.linspace(20,80,100), steinhart(np.linspace(20,80,100), *popt) - 298.15, 'g--')
plt.xlabel(r"Resistance - k$\Omega$")
plt.ylabel(r"Temperature - C$^{o}$")
plt.show()
