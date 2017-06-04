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
    
    double calculateRange(const vector<int> vec1, const vector<int> vec2);
    
    vector<vector<int> > trainObjects;
    
    double weight(const double range);
    
    int k;
};
#endif
