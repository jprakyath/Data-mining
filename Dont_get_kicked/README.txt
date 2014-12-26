This file explains how to run the two algorithms:

1. Naive Bayes Classifier:

There are 3 major source files for this classifier
	i. preprocess_data.m
	ii. csv2libsvm.py
	iii. NaiveBayes.py

i. preprocess_data.m:
	- This file is used to preprocess the data, First step is to read the training.csv and test.csv to 
the memory in MATLAB. We do this step manually and store the training.mat and test.mat. 
	- Using this training.mat and test.mat files, the training_preprocessed.csv and test_preprocessed.csv
are generated.
	- After this step we have to add IsBadBuy column to the test_preprocessed.csv to convert to LIBSVM format.
	- Then these files are required to generate the LIBSVM format files for the Naive Bayes classifier.
	
ii. csv2libsvm.py:
	- This file is used to generate LIBSVM format files from training_preprocessed.csv and test_preprocessed.csv
	- The command used to generate LIBSVM files are:
		python csv2libsvm.py -l 0 -s training_preprocessed.csv training.libsvm
		python csv2libsvm.py -l 0 -s test_preprocessed.csv test.libsvm
	- This will generate traning.dat and test.dat files
	- These files are input for NaiveBayes.py file.

iii. NaiveBayes.py:
	- This is the source file for Naive Bayes classifier.
	- The command used to generate submission_bayes.csv file is:
		python NaiveBayes.py traning.libsvm test.libsvm
	- This will generate the output file submission_bayes.csv. We submit this to get the score in Kaggle.

For convenience, we have included the traning.dat and test.dat files, after preprocessing the data. Note that, the NaiveBayes.py also expects the original test data in the current folder, to access the RefID column.

2. k-Nearest Neighbors

The source code files for k-NN is:
	i. kNN.py
	ii. minmax.py

	- The data is normalized using minmax normalization
	- This is done by the file minmax.py
	- To make things simpler, we have normalized the data and kept in the folder kNN
	( The minmax.py file requires the train_preprocessed.csv and test_preprocessed.csv files, They are not added 
	due to the space constraint )
	- To run the k-NN classifier, directly run
		python kNN.py
	- The program expects the following files to be in the same folder:
        	a. pre-processed-train.csv
	        b. pre-processed-test.csv
        	c. orig_test.csv

NOTE: The kNN program takes around 600 minutes to complete. One of the reason we decided to improve Naive Bayes 

