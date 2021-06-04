

from os import listdir
import numpy as np
import matplotlib.pyplot as plt
import csv
from matplotlib.ticker import MultipleLocator
from pandas.core.algorithms import unique
import seaborn as sns
import pandas as pd
from matplotlib.patches import PathPatch
import math
import seaborn as sns

def groupFiles():
    dic = {}
    
    files = listdir()
    
    files = filter(lambda x: x.endswith(".csv") and ("GNP" in x or "GIN" in x) and not x.startswith("result") and not x.startswith("usage"), files)
    
    for fil in files:
        key = fil.split("_")[0]
        if key in dic.keys():
            dic[key].append(fil)
        else:
            dic[key] = [fil]
    return dic

def getBestEachFile(files):
    best = []
    for file in files:
        best_fitnes = float('-inf')
        best_gen = -1
        with open(file, 'r') as f:
            reader = csv.reader(f, delimiter=';') 
            curr_gen = 0
            
            for line in reader:
                values = [float(v) for v in line[1:]]
                maxV = max(values)
                if maxV > best_fitnes:
                    best_fitnes = maxV
                    best_gen = curr_gen
                
                curr_gen+=1
            best.append((file, best_gen, best_fitnes))

    return best
        

def getBestFileGen(files):
    best_fitnes = float('-inf')
    best_gen = -1
    best_file = ''

    for file in files:
        with open(file, 'r') as f:
            reader = csv.reader(f, delimiter=';') 
            curr_gen = 0
            
            for line in reader:
                
                values = [float(v) for v in line[1:]]
                maxV = max(values)
                if maxV > best_fitnes:
                    best_fitnes = maxV
                    best_gen = curr_gen
                    best_file = file
                
                curr_gen+=1
    
    return (best_file, best_gen)
            
def do_magic(node_info, usage):
    dic = {}
    for node, count in zip(node_info, usage):
        dic[node] = dic.get(node, 0.0) + float(count)
    
    nodes=[]
    counts=[]
    for key in dic.keys():
        nodes.append(key)
        counts.append(dic[key])
    
    df = pd.DataFrame({"Nodes": nodes, 'Count': counts})


    ax = sns.barplot(x="Nodes", y='Count', data=df)
    plt.show()


def convert_node_info_core(n_info):
    type = n_info[0]
    index = n_info[len(n_info)-1]
    result = ''
    nodeName = {'0': 'Start', '1': 'J', '2': 'P', '3': 'NEAT'}
    
    result += nodeName[type]

    if(type == '1'):
        task = {
            '0': 'Left', 
            '1': 'Front',
            '2': 'Right',
            '3': 'Back',
            '4': 'Pheromone',
            '5': 'Nest',
            '6': 'Carry'
            }
        result += ':' + task[index]
    
    elif(type == '2'):
        task = {
            '0': 'Speed', 
            '1': 'Rotation',
            '2': 'Pheromone'
            }
        result += ':' + task[index]
    
    elif(type == '3'):
        result += index
    
    return result

def convert_node_info(node_info):
    result = [convert_node_info_core(x) for x in node_info]
    return result
    

def getUsage(file, gen):
    usage_file = 'usage_'+ file
        
    with open(usage_file) as u_file:
        data = csv.reader(u_file, delimiter=';')
        
        
        interestingrows=[row for idx, row in enumerate(data) if idx in (0, 1 + gen)]
        return (interestingrows[0], interestingrows[1])
        

def plotBars():
    files = groupFiles()
    
    for key in files.keys():
        (file, gen) = getBestFileGen(files[key])
        
        usage_file = 'usage_'+ file
        
        
        with open(usage_file) as u_file:
            data = csv.reader(u_file, delimiter=';')
            print(gen)
            
            interestingrows=[row for idx, row in enumerate(data) if idx in (0, 1 + gen)]
            node_info = convert_node_info(interestingrows[0])
            usage = interestingrows[1]

            print(node_info)
            print(usage)
            do_magic(node_info, usage)

def sum_nodes(node_info, usage, fitness):
    dic = {}
    for n, use in zip(node_info, usage):
        dic[n] = dic.get(n, 0) + float(use)
    node_info = unique(node_info)
    return (node_info, [dic[x] for x in node_info], fitness)

def heatMap():
    files = groupFiles()
    
    for key in files.keys():
        best = getBestEachFile(files[key])
        usages = [getUsage(file, gen) for file, gen, fitness in best]
        usages = [(inf, usage, fitness) for (inf, usage), (file, gen, fitness) in zip(usages, best)]
        
        usages = [sum_nodes(node_info, usage, fitness) for node_info, usage, fitness in usages]

        

        nodes = []
        runs = []
        uses = []

        i = 0
        for node_i, use, fitness in usages:
            node_i = convert_node_info(node_i)
            for n, u in zip(node_i, use):

                if not n == 'Start':
                    nodes.append(n)
                    uses.append(u)
                    runs.append(i)
            i+=1
        # usages = np.array([usage for _, usage, _ in usages]).T
        

        # n_info = convert_node_info(n_info)
        # dic = {}
        # for node, use in zip(n_info, usages):
        #     dic[node]=[float(v) for v in use]
        
        # print(len(n_info))
        # print(len(usages))
        # df = pd.DataFrame(dic)
        df = pd.DataFrame({
            'Nodes': nodes,
            'Uses': uses,
            'Runs': runs
        })

        cat = unique(nodes)
        height = 0.5 * len(cat)
        plt.figure(key, figsize=(12, height))
        df = df.pivot('Nodes', 'Runs', 'Uses')
        df.index = pd.CategoricalIndex(df.index, categories=cat)
        df.sort_index(level=0, inplace=True)
        
        ax = sns.heatmap(df,vmin=0)
        plt.tight_layout()
        plt.savefig(key + '.png')
    # plt.show()
        
def main():
    heatMap()
    

    



if __name__ == "__main__":
    plt.rcParams.update({'font.size': 16})

    main()
    