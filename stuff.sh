#! /bin/bash

cd /Users/trulsstenrud/Desktop/ExperimentsOfficial/experiment2

for i in $(find . -name "*" -type d)
do
cd $i 

/usr/local/bin/python3 /Users/trulsstenrud/Documents/GitHub/GNP-with-roborob3/roborobo3/pyScripts/pyScript.py; 

cd /Users/trulsstenrud/Desktop/ExperimentsOfficial/experiment2
done

cd /Users/trulsstenrud/Desktop/ExperimentsOfficial/experiment2

for algo in ./*
do
    cd $algo 
    
    for file in $(find ./cluster -name "result*" -type f)
    do
        cp $file ../../cluster
    done
    for file in $(find ./semi-random -name "result*" -type f)
    do
        cp $file ../../semi-random
    done
    for file in $(find ./random -name "result*" -type f)
    do
        cp $file ../../random
    done
    cd ..
done