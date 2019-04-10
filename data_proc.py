import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

fs = open("outputDat.txt", 'r')
data = [[float(i) for i in j[1:-1].split(',')] for j in fs.read().split('\n')[:-1] if float(j[1:-1].split(',')[0]) > 80]
fs.close()

temp = np.array([i[0] for i in data])
res = np.array([i[1] for i in data])

def func(x, a, b, c):
    return a * np.exp(-b * x) + c

#popt, pcov = curve_fit(func, temp, res)

coeff = np.polyfit(temp,(1/res), 1)
fit = np.poly1d(coeff)

#print(popt, pcov)

plt.plot(temp,1/(res), 'bo', ms=0.5)
#plt.plot(np.linspace(30,110,100), func(np.linspace(30,110,100), *popt), 'g--')
plt.plot((np.linspace(30,110,100)), (np.array([fit(i) for i in np.linspace(30,110,100)])), 'g--')
plt.show()
