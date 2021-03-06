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

def plotSwarmSizeLine():
    files = [file for file in getResultFiles() if 'max' in file]
    plotSwarmSize(files)
    plt.show()

def plotResult():
    files = getResultFiles()

    for file in files:
        plotFile(file)
    plt.show()

def boxResult():
    files = [file for file in getResultFiles() if 'max' in file]
    
    boxFiles(files)
    
    plt.show()

def plotAverageMax():
    files = [file for file in getResultFiles() if 'max' in file]
    
    plotFiles(files)
    
    plt.show()

def useStandardDeviation():
    files = [file for file in getResultFiles() if 'max' in file]
    doStandardDeviationStuff(files)
    

def getResultFiles():    
    files = listdir()
    files = list(filter(lambda x: x.endswith(".csv") and x.startswith("result"), files))
    return files

def doStandardDeviationStuff(files):
    
    dic = {}
    algos = []
    for file in files:
        nameSplit = file.rstrip(".csv").split("_")
        name = nameSplit[1]
        algos.append(name)
        data = np.genfromtxt(file, delimiter=";")
        
        data = np.amax(data, 0)
        dic[name] = data
        # for i in range(0, len(data[0])):
    
    
    l1 = dic[algos[0]]
    l2 = dic[algos[1]]
    m1 = np.mean(l1)
    m2 = np.mean(l2)
    v1 = np.var(l1)
    v2 = np.var(l2)
    n1 = n2 = 20

    a = (n1-1) * pow(v1, 2) + (n2 - 1) * pow(v2, 2)
    b = n1 + n2 - 2
    c = math.sqrt(1/n1 + 1/n2)

    t = (m1 - m2) / ( (a/b) * c )
    print(t)

def getBest(file):
    sum = 0
    data = np.genfromtxt(file, delimiter=";").T
    for line in data:
        sum += max(line)
    
    # return [sum/len(data)]
    return [max(x) for x in data]
    # return np.max(data)

def splitSwarmSizeAndName(text):
    algo = text.rstrip('1234567890')
    size = float(text[len(algo):len(text)])
    return (algo, size)

def plotSwarmSize(files):

    swarmSize = []
    fitness = []
    algos = []

    for file in files:
        for best_fitness in getBest(file):
            
            nameSplit = file.rstrip(".csv").split("_")
            name = nameSplit[1]
            (algo, size)  = splitSwarmSizeAndName(name)
            algos.append(algo)
            fitness.append(best_fitness)
            swarmSize.append(size)


    df = pd.DataFrame({'Swarm size': np.array(swarmSize),
                    'Fitness': np.array(fitness),
                    'Algorithm': np.array(algos)})
    
    df = df.sort_values(by=['Algorithm'])

    fig = plt.figure(figsize=(16, 10))
    sns.axes_style("whitegrid")

    ax = sns.lineplot(x='Swarm size', y="Fitness", hue="Algorithm", data=df, style="Algorithm", dashes=False, markers=True, markersize=10)
    
    ax.set(ylim=(0, 100))
    # ax.set(xscale='linear')
    #adjust_box_widths(fig, 0.6)
    plt.legend()
    plt.ylabel("% resources collected")
    
def plotFile(file):
    nameSplit = file.rstrip(".csv").split("_")
    name = nameSplit[1] + "_" + nameSplit[2]
    
    data = np.genfromtxt(file, delimiter=";")

    plt.figure(name)    
    plt.xlabel("Generations")
    
    plt.ylabel("Fitness")
    # plt.boxplot(data.T)
    plt.plot(data)

def plotFiles(files):

    algos = []
    max_gen = 0
    
    dic = {}
    for file in files:

        nameSplit = file.rstrip(".csv").split("_")
        name = nameSplit[1]
        (algo, _) = splitSwarmSizeAndName(name)
        algos.append(algo)
        data = np.genfromtxt(file, delimiter=";")
        max_gen = max(len(data), max_gen)
        dic[algo] = data

    interval = 1

    generations = list()
    algos = list()
    fitness = list()

    for algo in dic.keys():
        data = dic[algo]
        

        for i in range(0, len(data)):
            
            values = [x for x in data[i] if not math.isnan(x)]
            
            if False:
                for value in values:
                    algos.append(algo)
                    generations.append(i+1)
                    fitness.append(value)
            else:
                mean = np.median(values)
                
                algos.append(algo)
                generations.append(i+1)
                fitness.append(mean)
    
    
    df = pd.DataFrame({'Generations': np.array(generations),
                    'Fitness': np.array(fitness),
                    'Algorithm': np.array(algos)})
    df = df.sort_values(by=['Algorithm'])
    # Custom palette

    fig = plt.figure(figsize=(16, 10))
    sns.axes_style("whitegrid")

    ax = sns.lineplot(x='Generations', y="Fitness", hue="Algorithm", data=df, linewidth=1.5)
    # ax.axhline(96.875, ls='--', label='MPFA')
    ax.set(ylim=(0, 100))
    #adjust_box_widths(fig, 0.6)
    plt.legend()
    plt.ylabel("% resources collected")
    #ax.xaxis.set_minor_locator(MultipleLocator(0.5))
    #ax.xaxis.grid(True, which='minor', color='black', lw=2)
    
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
                
                
                for key in max_fitnesses:
                    
                    value = max_fitnesses[key]
                    algos.append(algo[0: len(algo)-2])
                    generations.append(i+1)
                    fitness.append(value)
                max_fitnesses = {}
        


   
    df = pd.DataFrame({'Generations': np.array(generations),
                    'Fitness': np.array(fitness),
                    'Algorithm': np.array(algos)})
    df = df.sort_values(by=['Algorithm'])
    # Custom palette
    my_pal = {"NEAT": "blue", "GNP": "red", 'NoveltySearch': 'green'}

    fig = plt.figure(figsize=(16, 10))
    sns.axes_style("whitegrid")
    ax = sns.boxplot(x='Generations', y="Fitness", hue="Algorithm", showmeans=True, data=df, #palette=my_pal, in the case that we want custom colors
                    meanprops={"marker": "s", "markerfacecolor": "white", "markeredgecolor": "black"}, width=0.8)
    
    adjust_box_widths(fig, 0.6)
    plt.ylabel("Fitness")
    ax.xaxis.set_minor_locator(MultipleLocator(0.5))
    ax.set(ylim=(0, 100))
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
    plt.rcParams.update({'font.size': 16})

    # plotAverageMax()
    # twoFiles = ['result_GNP++20_max.csv', 'result_GNP_max.csv']

    plotAverageMax()
    # boxResult()
    # plotSwarmSizeLine()
    
    # useStandardDeviation()
    # doStandardDeviationStuff(twoFiles)
    # boxResult()

    # data = np.array([[1, 2],[3, 4]])
    # for i in data:
    #     print(i)
    # test = np.amax(data, 0)
    # for i in test:
    #     print(i)
