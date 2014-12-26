#ifndef NAIVEBAYES_H
#define NAIVEBAYES_H

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

using namespace std;

#define APPLY_LAPLACE_CORRECTION

/*! \typedef 1D integer vector */
typedef vector<int> INT_VECTOR;

/*! \typedef 2D integer vector */
typedef vector< vector<int> >   INT_2D_VECTOR;

/*! \typedef 3D vector to store the probabilities */
typedef vector< vector< vector<double> > > PROBABILITY_TABLE;

/*! \class NaiveBayes
    \brief A class which contains the methods to read from LIBSVM file format,
    train the naive bayes classifier and test and print the perdicted results.
*/
class NaiveBayes
{
public:
    NaiveBayes();

    /*! \fn void getMaxAttribute(string file_name);
    \brief Function to get the maximum attribute in the testing and traning data file
    which are in LIBSVM format.
    \param file_name The name of the file which is to be read from.
    */
    void getMaxAttribute(string file_name);

    /*! \fn void getTrainingDataFromFile(string path);
    \brief Function to get the traning data from file and store it in class variables
    \param path The path of the file which is to be read from.
    */
    void getTrainingDataFromFile(string path);

    /*! \fn void getTestingDataFromFile(string path);
    \brief Function to get the testing data from file and store it in class variables
    \param path The path of the file which is to be read from.
    */
    void getTestingDataFromFile(string path);

    /*! \fn void train(const vector< vector<int> > & traindata, const vector<int> & trainClassValue);
    \brief Function to train the NaiveBayes classifier.
    \param traindata    Vector which contains the value of all the attributes in the traindata
    \param trainClassValue    Vector which contains the class value of all the attributes in the traindata(-1/1)
    */
    void train(const vector< vector<int> > & traindata, const vector<int> & trainClassValue);

    /*! \fn void predict(const vector< vector<int> > & testdata);
    \brief Function to classify the given test data to a class and output the predictions.
    \param testdata Vector which contains the value of all the attributes in the traindata.
    */
    void predict(const vector< vector<int> > & testdata);

    /*! \fn void calcuateMeasures(const vector<int> & label);
    \brief Function to calculate the result of the data.
        1. truePositive;
        2. trueNegative;
        3. falsePositive;
        4. falseNegative;
    \param label The vector which contains the original train data labels for the classes
    */
    void calcuateMeasures(const vector<int> & label);

    /*! \brief The vectores to hold the training and testing values */
    INT_2D_VECTOR trainset;
    /*! \brief The vectores to hold the training and testing values */
    INT_2D_VECTOR testset;
    /*! \brief The vectors to store the class labels of the training and testing data*/
    INT_VECTOR trainClassValue, testClassValue;

    /*! \brief class probability --> -1/1 **/
    double priorProb[2];
    /*! \brief The main data structure to store the probability of each attributes per class */
    PROBABILITY_TABLE probabilityTable;
    /*! \brief Vectors to store the result and the classcount and the Maximum value of the attributes */
    INT_VECTOR result, classCount, attributeValue;

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
};

#endif // NAIVEBAYES_H
