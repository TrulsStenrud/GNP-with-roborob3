# Plots the data in the "result" files produced by the pyScript.py
# run this script in the same folder as the result files

from os import listdir
import numpy as np
import matplotlib.pyplot as plt
import csv

def doStuff():
    files = getResultFiles()
    # plotFile(files[0])
    for file in files:
        plotFile(file)
    plt.show()

def getResultFiles():    
    files = listdir()
    files = list(filter(lambda x: x.endswith(".csv") and x.startswith("result"), files))
    return files

def plotFile(file):
    nameSplit = file.rstrip(".csv").split("_")
    name = nameSplit[1] + "_" + nameSplit[2]
    
    data = np.genfromtxt(file, delimiter=";")

    plt.figure(name)    
    plt.xlabel("Generations")
    plt.ylabel("Fitness")
    for test in data.T:
        plt.plot(test)
    

if __name__ == "__main__":
    doStuff()