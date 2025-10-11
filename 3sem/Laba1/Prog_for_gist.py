import numpy as np
import matplotlib.pyplot as plt

y = np.array([])

f = open('//wsl.localhost/Ubuntu-22.04/home/bbarannikov/MIPT_cpp/3sem/Laba1/Files_with_rolls/d100_double_without_data')
for line in f:
    y = np.append(y, int(line))
f.close()


plt.hist(y, bins = 100, color = 'skyblue', edgecolor = 'black')
plt.title('Распределение значений при броске двух d100 без множественного наследования')
plt.xlabel('Значения')
plt.ylabel('Частота')
plt.show()