# This file is for gathering data from the fitness log files from different runs
# Place the logfiles (NEAT_..., NoveltySearch_... etc.) in the same folder as this script and run this script from the same folder


from os import listdir
import numpy as np
import matplotlib.pyplot as plt
import csv
import itertools

def doStuff():
    dic = groupFiles()

    for key in dic.keys():
        generateSummaryFile(key, dic[key])
        
def generateSummaryFile(name, fileNames):
    files = [open(filename, 'r') for filename in fileNames]
    
    csvReaders = [csv.reader(file, delimiter=';') for file in files]
    
    maxName = "result_" + name + "_max" ".csv"
    meanName = "result_" + name + "_mean" + ".csv"
    
    with open(maxName, "w") as maxFile, open(meanName, "w") as meanFile:
        maxWriter = csv.writer(maxFile, delimiter=";")
        meanWriter = csv.writer(meanFile, delimiter=";")

        # read all the files in paralell
        for lines in itertools.zip_longest(*csvReaders,  fillvalue=["Generation x: ", float('nan')]):
            numbers = [[float(value) for value in line[1:]] for line in lines]
            maxValues = [max(values) for values in numbers]    
            meanValues = [sum(values)/len(values) for values in numbers]    
            maxWriter.writerow(maxValues)
            meanWriter.writerow(meanValues)
    
    for file in files:
        file.close()



def groupFiles():
    dic = {}
    
    files = listdir()
    files = filter(lambda x: x.endswith(".csv") and not x.startswith("result"), files)
    
    for fil in files:
        key = fil.split("_")[0]
        if key in dic.keys():
            dic[key].append(fil)
        else:
            dic[key] = [fil]
    return dic
    

if( __name__ == "__main__"):
    doStuff()