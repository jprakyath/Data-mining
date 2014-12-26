#ifndef ADABOOST_H
#define ADABOOST_H

#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include <climits>
#include <time.h>

#define K 6

#define APPLY_LAPLACE_CORRECTION

using namespace std;

/*! \typedef 1D integer vector */
typedef vector<int> INT_VECTOR;

/*! \typedef 1D integer vector */
typedef vector<double> DOUBLE_VECTOR;

/*! \typedef 2D integer vector */
typedef vector< vector<int> >   INT_2D_VECTOR;

/*! \typedef 4D vector to store the probabilities */
typedef vector<vector<vector<vector<double> > > > PROBABILITY_TABLE;

/*! \class Adaboost
    \brief A class which contains the methods to read from LIBSVM file format,
    train the Adaboost classifier and test and print the perdicted results.
*/
class Adaboost {

    public:
        /*! \brief Variables to store the traning data, testing data and the sampling data*/
        INT_2D_VECTOR	trainset, testset, sampleset;

        /*! \brief Variable to store the training class values, testing class values and the sample class values*/
        INT_VECTOR trainClassValue, testClassValue, sampleClassValue;

        /*! \brief Variable to store the attribute size*/
        int attributeSize;
        /*! \brief Variable to store the true postive values */
        int truePositive;
        /*! \brief Variable to store the true Negative values */
        int trueNegative;
        /*! \brief Variable to store the false postive values */
        int falsePositive;
        /*! \brief Variable to store the false Negative values */
        int falseNegative;

        /*! \brief class probability --> -1/1 **/
        double priorProb[K][2];
        /*! \brief The main data structure to store the probability of each attributes per class */
        PROBABILITY_TABLE probabilityTable;

        /*! \brief The data structure to store the result of each naive bayes run and the to store
         * the attribute value for each naive bayes run */
        INT_2D_VECTOR result, attributeValue;
        /*! \brief The data structure to store the classCount and the result of the adaboost run */
        INT_VECTOR classCount, adaResult;
        /*! \brief Data structures to store the weights of each tuples and the error for each naive bayes run */
        DOUBLE_VECTOR weight, error;

        Adaboost();

        /*! \fn void getMaxAttribute(string file_name);
        \brief Function to get the maximum attribute in the testing and traning data file
        which are in LIBSVM format.
        \param file_name The name of the file which is to be read from.
        */
        void getMaxAttribute(string file_name);

        /*! \fn  void 	getDataFomFiles(string trainingFile, string testingFile);
        \brief Function to get the testing data from file and store it in class variables
        \param trainingFile The path of the training file which is to be read from.
        \param testingFile The path of the testing file which is to be read from.
        */
        void getDataFomFiles(string trainingFile, string testingFile);

        /*! \fn void train(const vector< vector<int> > & traindata, const vector<int> & trainClassValue, int k);
        \brief Function to train the NaiveBayes classifier.
        \param traindata    Vector which contains the value of all the attributes in the traindata
        \param trainClassValue    Vector which contains the class value of all the attributes in the traindata(-1/1)
        \param k    the number representing the classifier (0-7)
        */
        void train(const vector< vector<int> > & traindata, const vector<int> & trainClassValue, int k);

        /*! \fn void predict(const vector< vector<int> > & testdata, int k);
        \brief Function to classify the given test data to a class and output the predictions.
        \param testdata Vector which contains the value of all the attributes in the traindata.
        \param k    the number representing the classifier (0-7)
        */
        void predict(const vector< vector<int> > & testdata, int k);

        /*! \fn void generateRandomSamples(const vector< vector<int> > & data , vector<int>& classValue);
        \brief Function to generate ramdon samples from the given training data.
        \param data Vector which contains the value of all the attributes in the traindata.
        \param classValue The value of the class that the samples belong to
        */
        void generateRandomSamples(const vector< vector<int> > & data , vector<int>& classValue);

        /*! \fn void adaBoostTrain(const vector< vector<int> > & data, const vector<int> & classValue);
        \brief Function to train the NaiveBayes classifier.
        \param data    Vector which contains the value of all the attributes in the traindata
        \param classValue    Vector which contains the class value of all the attributes in the traindata(-1/1)
        */
        void adaBoostTrain(const vector< vector<int> > & data, vector<int> & classValue);

        /*! \fn void adaBoostPredict(const vector< vector<int> > & data);
        \brief Function to classify the given test data to a class and output the predictions.
        \param testdata Vector which contains the value of all the attributes in the traindata.
        */
        void adaBoostPredict(const vector< vector<int> > & data);

        /*! \fn void calcuateMeasures(const vector<int> & classValue);
        \brief Function to calculate the result of the data.
            1. truePositive;
            2. trueNegative;
            3. falsePositive;
            4. falseNegative;
        \param classValue The vector which contains the original train data labels for the classes
        */
        void calcuateMeasures(const vector<int> & classValue);

};

#endif // ADABOOST_H
