#ifndef KNNCLASSIFIER_H
#define KNNCLASSIFIER_H

#include "ClassificationAlgorithm.h"

#include <vector>

using namespace std;

class KNNClassifier: public ClassificationAlgorithm
{
public:
    
    KNNClassifier(const vector<vector<int> > aMatrix, const int ak);
    
    vector<double> predict(const vector<int> testObject);
    
private:
    
    double calculateRange(const vector<double> vec1, const vector<double> vec2);
    
    double weight(const double range);
    
    void normalize(vector<double>& vec);
    
    int k;
    
    vector<vector<double> > trainObjects;
    vector<int> classLabels;
};
#endif
