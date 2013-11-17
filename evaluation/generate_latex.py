#!/usr/bin/env python3

import sys
import matplotlib.pyplot as plt
from pprint import pprint
import matplotlib as mpl


with open(sys.argv[1]) as f:
    data = f.readlines()

data.pop(0)
data.pop(-1)
vertices = [int(row.split()[0].split('_')[0]) for row in data]
edges = [int(row.split()[0].split('_')[1]) for row in data]
sequence = [float(row.split()[1]) for row in data]
parallel = [float(row.split()[2]) for row in data]
speedup = [float(row.split()[3]) for row in data]

print("\\toprule")
print("Vrcholů & Hran & Sekvenční [s] & Parelelní [s] & Zrychlení\\\\\\midrule")
for i in range(len(vertices)):
    print("%d & %d & %.3f & %.3f & %.3f\\\\"%(vertices[i], edges[i], sequence[i], parallel[i], speedup[i]))
print("\\bottomrule")
