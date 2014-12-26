#include "adaboost.h"

Adaboost::Adaboost(){
    /* Initialize variables */
    truePositive = 0;
    trueNegative = 0;
    falsePositive = 0;
    falseNegative = 0;
    attributeSize = 0;
}

/* Function used to get the maximum attribute in both the test file & train file*/
void Adaboost::getMaxAttribute(string file_name){
    /* Read the file */
    ifstream filein(file_name.c_str());
    set<int> attributes;
    if(filein.is_open()) {
        string line;
        while (!filein.eof()){
            getline(filein, line, '\n');
            if(line == "") break;
            istringstream linestr(line);
            string temp;
            while(linestr >> temp){
                /* Get the maximum attribute */
                attributes.insert(atoi(temp.substr(0, temp.find(":")).c_str()));
            }
        }
    }
    filein.close();
    /* This is to check if the training data or test data has max number of attributes*/
    if(this->attributeSize < (*attributes.rbegin()));
        /*If the attribute is greater in the testing file replace the attribute */
        this->attributeSize = (*attributes.rbegin());
 #ifdef DEBUG_LOG
    cout << "Attributes size " << this->attributeSize <<endl;
#endif
}

/* Function used to get the data from both the test file & train file*/
void Adaboost::getDataFomFiles(string trainingFile, string testingFile){

    ifstream trainFile(trainingFile.c_str());
    if(trainFile.is_open()) {
        string line;
        while (!trainFile.eof()){
            getline(trainFile, line, '\n');
            if(line == "") break;
            istringstream linestr(line);
            vector<int> tuple;
            int classValue;
            linestr >> classValue;
            /* Getting the class label for train data */
            this->trainClassValue.push_back((classValue < 0? 0:1));
            string temp;
            for(int i = 0; i < this->attributeSize; i++){
                linestr >> temp;
                int pos = temp.find(":");
                /* Get the attribute */
                int attribute = atoi(temp.substr(0, pos).c_str());
                /* Get the value of the attribute */
                int value = atoi(temp.substr(pos + 1, temp.length()).c_str());
                while(attribute-1 != i && i < this->attributeSize){
                    tuple.push_back(0);
                    i++;
                }
                if( i >= this->attributeSize) break;
                tuple.push_back(value);
            }
            /* Put the values in the vector */
            this->trainset.push_back(tuple);
        }
    }
    trainFile.close();

    ifstream testFile(testingFile.c_str());
    if(testFile.is_open()) {
        while (!testFile.eof()){
            string line;
            getline(testFile, line, '\n');
            if(line == "") break;
            istringstream linestr(line);
            vector<int> tuple;
            int classValue;
            linestr >> classValue;
            /* Getting the class label for test data */
            this->testClassValue.push_back((classValue < 0? 0:1));
            for(int i = 0; i < this->attributeSize; i++){
                string temp;
                linestr >> temp;
                int pos = temp.find(":");
                /* Get the attribute */
                int attribute = atoi(temp.substr(0, pos).c_str());
                /* Get the value of the attribute */
                int value = atoi(temp.substr(pos + 1, temp.length()).c_str());
                while(attribute-1 != i && i < this->attributeSize){
                    tuple.push_back(0);
                    i++;
                }
                if( i >= this->attributeSize) break;
                tuple.push_back(value);
            }
            /* Put the values in the vector */
            this->testset.push_back(tuple);
        }
    }
    testFile.close();
}

/* This is the same function used in naive bayes but the data stored is for K runs */
void Adaboost::train(const vector< vector<int> > & traindata, const vector<int> & trainClassValue, int k) {
    this->classCount.clear();
    INT_VECTOR init;
    vector<vector<vector<double> > > probabilityTableForK;

    for (int i =0; i < this->attributeSize; i++) {
        init.push_back(0);
    }
    attributeValue.push_back(init);
#ifdef DEBUG_LOGS
    cout << "Train data size " << traindata.size() <<endl;
    cout << "Attribute size " << this->attributeSize <<endl;
#endif
    /* Iterate the loop till the max attribute for each tuple */
    for (int i = 0; i < this->attributeSize; i++) {
        /* Iterate the loop till the size of the training data (number of tuples) */
        for (int j = 0; j < traindata.size(); j++) {
            /* Finding the maximum value of an attribute */
             if(traindata[j][i] > attributeValue[k][i]){
                attributeValue[k][i] = traindata[j][i];
#ifdef DEBUG_LOGS
                cout<< "Attribute index is "<< i+1 << " Value of the Attribute is " << attributeValue[k][i] <<endl;
#endif
            }
        }
    }

    /* Initialize the probability table --> probabilityTable */
    /* j = 2 --> number of classes is 2 in our case (+1/-1) */
    for (int j = 0; j < 2; j++) {
        /* dummy vector to fill the probability table with 0's */
        vector< vector<double> > temp1;
        for (int i =0; i<this->attributeSize; i++) {
            /* dummy vector to fill the probability table with 0's */
            vector<double> temp2;
            for (int h =0; h < attributeValue[k][i]+1; h++) {
                temp2.push_back(0.0);
            }
            temp1.push_back(temp2);
        }
        probabilityTableForK.push_back(temp1);
        /* Vector with number of classes */
        this->classCount.push_back(0);
    }

    /* Iterate the loop till the size of the training data (number of tuples) */
    for (int j = 0; j < traindata.size(); j++) {
        /* Iterate the loop till the max attribute for each tuple */
        for (int i = 0; i < this->attributeSize; i++) {
            /* trainClassValue = 0/1 --> (-1,1) class */
            /* find the cummulative probability of each attribute in the data
             *  probabilityTable[0][attribute][attribute_value]++;
            */
            probabilityTableForK[trainClassValue[j]][i][traindata[j][i]] += 1.0;
        }
        /* Store the value of the actual number of variables in the class */
        this->classCount[trainClassValue[j]]++;
#ifdef DDEBUG_LOGS
        cout << "The value of the classcount of class " << trainClassValue[j] << " is " <<this->classCount[trainClassValue[j]]<<endl;
#endif
    }

    int zeroCount = 0;
    /* Class value - 0/1 */
    for (int j = 0; j < 2; j++) {
        /* For each attribute in tuple */
        for (int i =0; i<this->attributeSize; i++) {
            for(int f = 0; f < attributeValue[k][i]+1; f++){
                /* probability of each attribute for the two classes 0/1
                 * attribute_value/total_class_attributes
                */
#ifdef APPLY_LAPLACE_CORRECTION
                if(probabilityTableForK[j][i][f] == 0){
                    /* applying Laplace correction */
#ifdef DEBUG_LOGS
                    cout <<"Zero!!!"<<endl;
#endif
                    zeroCount++;
                    probabilityTableForK[j][i][f] += 1;
                    probabilityTableForK[j][i][f] = probabilityTableForK[j][i][f]/
                                                    ((double)classCount[j]+attributeValue[k][i]);
                } else {
#endif
                    probabilityTableForK[j][i][f] = probabilityTableForK[j][i][f]/
                                                    (double)classCount[j];
#ifdef APPLY_LAPLACE_CORRECTION
                }
#endif
            }
        }
        priorProb[k][j] = ((double)this->classCount[j]/(double)traindata.size());
    }
#ifdef DEBUG_LOGS
    //cout <<"Zero Count :"<< zeroCount <<endl;
#endif
    this->probabilityTable.push_back(probabilityTableForK);
}

/* This is the same function used in naive bayes but involves an extra step to
 calculate result of the adaboost run */
void Adaboost::predict(const vector< vector<int> > & data, int k) {
    this->result[k].clear();
    vector<int> sample;
    for (int s = 0; s<data.size(); s++) {
        sample = data[s];
        long double postiriori[2];
        /* calculating postiriori probability for 2 classes*/
        for (int i = 0 ; i< 2; ++i){
            postiriori[i] = (this->priorProb[k][i]);
            for(int f = 0; f < attributeSize; ++f) {
                long double attributeProb;
                attributeProb = this->probabilityTable[k][i][f][sample[f]];
                /* postiriori_probability = product (prior_probability * probability_of_attribute) */
                postiriori[i] *= (attributeProb);
            }
        }
        /* classify to class 0 or 1*/
        if(postiriori[0] != 0 && postiriori[1] != 0){
#ifdef DEBUG_LOGS
            cout << "Class -1 :"<<postiriori[0]<< " Class 1 :"<<postiriori[1]<<endl;
#endif
            int classified_value = postiriori[0] > postiriori[1] ? 0 : 1;
            this->result[k].push_back(classified_value);
        }
    }
}


/* Function to generate ramdom samples from the train data */
void Adaboost::generateRandomSamples(const vector< vector<int> > & data, vector<int>& classValue) {

    INT_2D_VECTOR  sample;
    INT_VECTOR sampleClassValue;
    /* Clear the sample set generated for each run */
    this->sampleset.clear();
    this->sampleClassValue.clear();

    double tempVal =0;
    double randomNum =0;
    for (int i =0; i<data.size(); i++) {
        /* generate a random number */
        randomNum = rand()/double(INT_MAX);
#ifdef DEBUG_LOGS
        cout << "Generated random number is :"<< randomNum<<endl;
#endif
        tempVal = 0;
        for (int j =0; j<data.size(); j++) {
            tempVal+= weight[j];
            if (tempVal > randomNum) {
                /* Get the ramdon generated tuple and store the result */
                sample.push_back(data[j]);
                sampleClassValue.push_back(classValue[j]);
                break;
            }
        }
    }
    this->sampleset = sample;
    this->sampleClassValue = sampleClassValue;
}

/* Training function of the adaboost algorithm */
void Adaboost::adaBoostTrain(const vector< vector<int> > & data, vector<int> & classValue){

    /* Step 1: assign 1/d as weights of the tuples */
    for(int i = 0; i < classValue.size(); i++){
        this->weight.push_back(1);
    }

    /* Assign the weight of each tuple as 1/d */
    double sumOfWeight = 0;
    for(int i = 0; i < data.size(); i++){
        sumOfWeight += this->weight[i];
    }
    for(int i = 0; i < data.size(); i++){
        this->weight[i] = this->weight[i] / (double)sumOfWeight;
    }

    /* Initialize the initial predicted result to zero */
    for(int k = 0; k < K; k++){
        vector<int> temp;
        for (int i = 0; i < this->sampleClassValue.size(); i++)
            temp.push_back(0);
        this->result.push_back(temp);
    }

    /* Step 2: for k times run the naive bayes classifier */
    for(int k = 0; k < K; k++){
        /* Step 3: get a random sample of the data */
        generateRandomSamples(data ,classValue);

        /* Step 4: train the sample; naive bayes classifier */
        train(this->sampleset, this->sampleClassValue, k);

        /* Step 5: test the sample data using the naive bayes classifier */
        predict(this->sampleset, k);

        /* Step 6: compute the error of the data */
        double err = 0.0f;
        for(int i = 0; i < this->sampleset.size(); i++){
            if(this->sampleClassValue[i] != this->result[k][i]){
                err += this->weight[i];
            }
        }
        this->error.push_back(err);

        /* Step 7: if the error computed is greater than 0.5 rerun the classifier */
        if(this->error[k] > 0.5){
            this->probabilityTable.pop_back();
            this->error.pop_back();
            this->attributeValue.pop_back();
            k--;
            continue;
        }

        /* Step 8: Update the weight of the classified data */
        DOUBLE_VECTOR updateWeight;
        for (int i = 0; i < this->sampleClassValue.size(); i++) {
            /* For the values which are correctly classified update the weight */
            if(this->sampleClassValue[i] == this->result[k][i]){
                /* updating the weight */
                updateWeight.push_back( (this->weight[i] * this->error[k]) / (1.0f-this->error[k]));
            }
            else{
                /* If the data is not correctly classified don't update the weight */
                updateWeight.push_back( this->weight[i]);
            }
        }
        this->weight.clear();
        /* Updated weight */
        this->weight = updateWeight;

        /* Step 9: Normalize the weight */
        sumOfWeight = 0;
        for(int i = 0; i < data.size(); i++){
            sumOfWeight += this->weight[i];
        }
        for(int i = 0; i < data.size(); i++){
            /* Normalizing the weight by dividing the weight generated by the sum of weights */
            this->weight[i] = this->weight[i] / (double)sumOfWeight;
        }
    }
}


/* This is the adaboost predict function which stores the classified result */
void Adaboost::adaBoostPredict(const vector< vector<int> > & data) {
    this->adaResult.clear();
    INT_VECTOR sample;
    for (int s = 0; s<data.size(); s++) {
        sample = data[s];
        double classWeight[2] = {0.0f,0.0f};
        for(int k = 0; k < K; k++){
            double reweightValue = log((1.0f - this->error[k])/this->error[k]);
            long double postiriori[2];
            for (int i = 0 ; i< 2; ++i){
                postiriori[i] = (this->priorProb[k][i]);
                for(int f = 0; f < attributeSize; ++f) {
                    long double attributeProb;
                    attributeProb = this->probabilityTable[k][i][f][sample[f]];
                    /* postiriori_probability = product (prior_probability * probability_of_attribute) */
                    postiriori[i] *= (attributeProb);
                }
            }
            int classified_value = postiriori[0] > postiriori[1] ? 0 : 1;
            classWeight[classified_value] += reweightValue;
        }
        int result = classWeight[0] > classWeight[1]? 0 : 1;
        this->adaResult.push_back(result);
    }
}

/* This is the same function used in the Naive bayes classifier */
void Adaboost::calcuateMeasures(const vector<int> & classValue){
    /* Initialize the measures to zero */
    truePositive = 0;
    trueNegative = 0;
    falsePositive = 0;
    falseNegative = 0;
    /* calculation of the measures */
    for (int n = 0; n < this->adaResult.size(); n++) {
        if(classValue[n] == 0){
            if(this->adaResult[n] == 0){
                trueNegative +=1;
            } else {
                falsePositive +=1;
            }
        } else {
            if(this->adaResult[n] == 1){
                truePositive +=1;
            } else {
                falseNegative +=1;
            }
        }
    }
    cout<<truePositive<<" "<<falseNegative<<" "<< falsePositive<<" "<<trueNegative<<endl;
#ifdef DEBUG_LOGS
    double accuracy = (double)(truePositive + trueNegative)/
                      (truePositive + trueNegative + falsePositive + falseNegative);
    double error = (double)(falsePositive + falseNegative)/
            (truePositive + trueNegative + falsePositive + falseNegative);
    double sensitivity = (double)(truePositive)/(truePositive + falseNegative);
    double specificity = (double)(trueNegative)/(trueNegative + falsePositive);
    double precision = (double)(truePositive)/(truePositive + falsePositive);
    double 	recall = sensitivity;
    double  f1 = 2 * (precision * recall) / (precision + recall);
    double  fhalf = (1 + 0.5*0.5) * (precision * recall) / (0.5*0.5 * precision + recall);
    double  f2 = (1 + 2*2) * (precision * recall) / (2*2 * precision + recall);
    cout<<"Accuracy: "<<accuracy<<endl;
    cout<<"Error Rate: "<<error<<endl;
    cout<<"Sensitivity: "<<sensitivity<<endl;
    cout<<"Specificity: "<<specificity<<endl;
    cout<<"Precision: "<<precision<<endl;
    cout<<"F-1 Score: "<<f1<<endl;
    cout<<"F-0.5 Score: "<<fhalf<<endl;
    cout<<"F-2 Score: "<<f2<<endl;
#endif
}

/* The main function */
int main(int argc, char* argv[]) {

    if (argc != 3){
            cout<< "Use format: ./NBAdaBoost training_file test_file" << endl;
            exit(1);
    }

    string training_data = (string)argv[1];
    string testing_data = (string)argv[2];

    Adaboost adaBoost;

    adaBoost.getMaxAttribute(training_data);
    adaBoost.getMaxAttribute(testing_data);

    adaBoost.getDataFomFiles(training_data,testing_data);
    srand (time(NULL));
    adaBoost.adaBoostTrain(adaBoost.trainset, adaBoost.trainClassValue);

    adaBoost.adaBoostPredict(adaBoost.trainset);
    adaBoost.calcuateMeasures(adaBoost.trainClassValue);

    adaBoost.adaBoostPredict(adaBoost.testset);
    adaBoost.calcuateMeasures(adaBoost.testClassValue);

}
