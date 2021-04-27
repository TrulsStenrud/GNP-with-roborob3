# Plots the data in the "result" files produced by the pyScript.py
# run this script in the same folder as the result files

from os import listdir
import numpy as np
import matplotlib.pyplot as plt
import csv
from matplotlib.ticker import MultipleLocator
import seaborn as sns
import pandas as pd
from matplotlib.patches import PathPatch
import math

def plotResult():
    files = getResultFiles()

    for file in files:
        plotFile(file)
    plt.show()

def boxResult():
    files = [file for file in getResultFiles() if 'max' in file]
    
    boxFiles(files)
    
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
    # plt.boxplot(data.T)
    plt.plot(data)

def boxFiles(files):

    algos = []
    max_gen = 0
    
    dic = {}
    for file in files:

        nameSplit = file.rstrip(".csv").split("_")
        name = nameSplit[1]
        algos.append(name)
        data = np.genfromtxt(file, delimiter=";")
        max_gen = max(len(data), max_gen)
        dic[name] = data

    interval = int(max_gen / 5)

    generations = list()
    algos = list()
    fitness = list()

    for algo in dic.keys():
        data = dic[algo]
        max_fitnesses = {}

        for i in range(0, len(data)):
            currLine = data[i]
            for j in range(0, len(currLine)):
                currVal = float(currLine[j])
                if not math.isnan(currVal):
                    max_fitnesses[j] = max(max_fitnesses.get(j, 0), currVal)
            
            if((i+1) % interval == 0):
                
                print(max_fitnesses)
                for key in max_fitnesses:
                    
                    value = max_fitnesses[key]
                    algos.append(algo)
                    generations.append(i+1)
                    fitness.append(value)
                max_fitnesses = {}
        


    
    df = pd.DataFrame({'Generations': np.array(generations),
                    'Fitness': np.array(fitness),
                    'Algorithm': np.array(algos)})
    
    # Custom palette
    my_pal = {"NEAT": "blue", "GNP": "red", 'NoveltySearch': 'green'}

    fig = plt.figure(figsize=(16, 10))
    sns.axes_style("whitegrid")
    ax = sns.boxplot(x='Generations', y="Fitness", hue="Algorithm", showmeans=True, data=df, #palette=my_pal, in the case that we want custom colors
                    meanprops={"marker": "s", "markerfacecolor": "white", "markeredgecolor": "black"}, width=0.8)
    
    adjust_box_widths(fig, 0.6)
    plt.ylabel("Best fitness so far")
    ax.xaxis.set_minor_locator(MultipleLocator(0.5))
    ax.xaxis.grid(True, which='minor', color='black', lw=2)
    


def adjust_box_widths(fig, fac):
    """
    Adjust the widths of a seaborn-generated boxplot.
    """

    # iterating through Axes instances
    for ax in fig.axes:

        # iterating through axes artists:
        for c in ax.get_children():

            # searching for PathPatches
            if isinstance(c, PathPatch):
                # getting current width of box:
                p = c.get_path()
                verts = p.vertices
                verts_sub = verts[:-1]
                xmin = np.min(verts_sub[:, 0])
                xmax = np.max(verts_sub[:, 0])
                xmid = 0.5*(xmin+xmax)
                xhalf = 0.5*(xmax - xmin)

                # setting new width of box
                xmin_new = xmid-fac*xhalf
                xmax_new = xmid+fac*xhalf
                verts_sub[verts_sub[:, 0] == xmin, 0] = xmin_new
                verts_sub[verts_sub[:, 0] == xmax, 0] = xmax_new

                # setting new width of median line
                for l in ax.lines:
                    if np.all(l.get_xdata() == [xmin, xmax]):
                        l.set_xdata([xmin_new, xmax_new])
if __name__ == "__main__":
    plotResult()
