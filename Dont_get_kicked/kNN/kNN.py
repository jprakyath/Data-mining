import csv
import random
import math
import operator
import time
import os
import pandas as pd

def loadTrainingDataset(filename, trainingSet=[]):
    lines = csv.reader(open(filename, 'rU'))
    dataset = list(lines)
    labels = dataset.pop(0)
    num_attrs = len(labels)

    for x in range(len(dataset) - 1):
        for y in range(1, num_attrs):
            dataset[x][y] = float(dataset[x][y])
        trainingSet.append(dataset[x])

def loadTestDataset(filename, testSet=[]):
    lines = csv.reader(open(filename, 'rU'))
    dataset = list(lines)
    labels = dataset.pop(0)
    num_attrs = len(labels)

    for x in range(len(dataset)):
        for y in range(num_attrs):
            dataset[x][y] = float(dataset[x][y])
        testSet.append(dataset[x])

def euclideanDistance(instance1, instance2, length):
    distance = 0
    for x in range(length):
        distance += pow((instance1[x] - instance2[x]), 2)
    return math.sqrt(distance)

def getNeighbors(trainingSet, testInstance, k):
    distances = []
    length = len(testInstance) - 1
    for x in range(len(trainingSet)):
        dist = euclideanDistance(testInstance, trainingSet[x][1:], length)
        distances.append((trainingSet[x], dist))
    distances.sort(key=operator.itemgetter(1))
    neighbors = []
    for x in range(k):
        neighbors.append(distances[x][0])
    return neighbors

def getResponse(neighbors):
    classVotes = {}
    for x in range(len(neighbors)):
        response = neighbors[x][0]
        if response in classVotes:
            classVotes[response] += 1
        else:
            classVotes[response] = 1
    if '1.0' in classVotes.keys():
        if classVotes['1.0'] >= ( len(neighbors)/ 4):
            return int(1)
        else:
            return int(0)
    else:
        return int(0)
    
    # FIXME : Trying to return 1, if even 1/4th points near are those
    #sortedVotes = sorted(classVotes.iteritems(), key=operator.itemgetter(1), reverse=True)
    #return int ( ((sortedVotes[0][0]).split('.'))[0] )

def read_file(file_name, header=False):
    f = open(file_name)
    reader = csv.reader(f)
    if not header:
        reader.next()
    res = [line for line in reader]
    f.close()
    return res

def main():
    start_time = time.time()
    
    # prepare data
    trainingSet = []
    testSet = []
    loadTrainingDataset( 'pre-processed-train.csv', trainingSet)
    loadTestDataset( 'pre-processed-test.csv', testSet)
    print 'Train set: ' + repr(len(trainingSet))
    print 'Test set: ' + repr(len(testSet))
    
    # generate predictions
    decisions = []
    k = 8
    total_size = len(testSet)
    print "Predicting now => "
    for x in range(total_size):
        if ( ( x % ( total_size / 100) ) == 0 ):
            print x

        neighbors = getNeighbors(trainingSet, testSet[x], k)
        result = getResponse(neighbors)
        decisions.append(result)

    print 'Writing submission file...'
    # Print the submission file
    get_refid = pd.read_csv('orig_test.csv')
    predictions = pd.Series(decisions)
    print predictions.value_counts()
    submission = pd.DataFrame({'RefId': get_refid.RefId, 'IsBadBuy': predictions})
    cols = submission.columns.tolist()
    cols = cols[-1:] + cols[:-1]
    submission = submission[cols]
    submission.to_csv('submission_kNN.csv', index=False)

    print (time.time() - start_time) / 60.0, 'minutes'

main()
