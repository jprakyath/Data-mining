import pandas as pd
import numpy as np
from sklearn import preprocessing

train = pd.read_csv('training_preprocessed.csv')
for x in train:
    if x == 'IsBadBuy':
        continue
    max_val = max(train.loc[:,x])
    min_val = min(train.loc[:,x])
    for i in range(len(train)):
        val = (train.loc[i,x] - min_val)/float(max_val - min_val)
        train.loc[i,x] = val
train.to_csv('pre-processed-train.csv', index=False)

test = pd.read_csv('test_preprocessed.csv')
for x in test:
    if x == 'IsBadBuy':
        continue
    max_val = max(test.loc[:,x])
    min_val = min(test.loc[:,x])
    for i in range(len(test)):
        val = (test.loc[i,x] - min_val)/float(max_val - min_val)
        test.loc[i,x] = val
test.to_csv('pre-processed-test.csv', index=False)

