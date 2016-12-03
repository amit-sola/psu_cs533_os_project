import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
import pdb

#filename = "output/numArray.txt"
#varyIdx = 2
#title = "Size of Array vs Time\nkthreads:2\nseq_threshold:100"
#xaxis = "Number of elements"
#yaxis = "Real time"
#outFilename = "output/arraysize.png"

#filename = "output/seqThreshold.txt"
#varyIdx = 3
#title = "Sequence Threshold vs Time\nkthreads:2\nnumArray:3000000"
#xaxis = "Sequence Threshold"
#yaxis = "Real time"
#outFilename = "output/seq_threshold.png"

filename = "output/numThreads.txt"
varyIdx = 1
title = "Number of kthreads vs Time\nnumArray:3000000\nseq_threshold:200"
xaxis = "Sequence Threshold"
yaxis = "Real time"
outFilename = "output/numThreads.png"

f = open(filename, 'r')
lines = f.readlines()
f.close()

#Each run is 5 lines
#<cmd>
#\n
#<real time>
#<user time>
#<sys time>
numSamples = len(lines)/5
x = np.zeros((numSamples))
y = np.zeros((numSamples))

for i in range(numSamples):
    ln = i*5
    x[i] = float(lines[ln].split(" ")[varyIdx])
    y[i] = float(lines[ln+2].split("\t")[1][2:-2])

plt.plot(x, y)
plt.title(title)
plt.xlabel(xaxis)
plt.ylabel(yaxis)
plt.subplots_adjust(top=0.85)
plt.savefig(outFilename)
