#!/usr/bin/env python3

import sys
import matplotlib.pyplot as plt
from pprint import pprint
import matplotlib as mpl


with open(sys.argv[1]) as f:
    data = f.readlines()

data.pop(0)
data.pop(-1)
vertices = [float(row.split()[0].split('_')[0]) for row in data]
sequence = [float(row.split()[1]) for row in data]
parallel = [float(row.split()[2]) for row in data]
speedup = [float(row.split()[3]) for row in data]

plt.figure(1, figsize=(10, 5))
plt.subplot(211)
plt.plot(vertices, sequence, vertices, parallel, label = ("Sekvenční", "Paralelní"))
plt.legend(("Sekvenční", "Paralelní"), loc='upper left')
plt.ylabel("Čas [s]")
plt.xlabel("Počet vrcholů grafu")
plt.title("Čas zpracovávání")

plt.subplot(212)
plt.plot(vertices, speedup)
plt.ylabel("Zrychlení")
plt.xlabel("Počet vrcholů grafu")
plt.title("Zrychlení")

plt.tight_layout()


if (len(sys.argv) >= 3):
    from matplotlib import rc
    rc('font',**{'family':'serif','serif':['Computer Modern Roman']})
    rc('text', usetex=True)
    mpl.rcParams['text.latex.unicode']=True
    plt.savefig(sys.argv[2])
else:
    plt.show()


