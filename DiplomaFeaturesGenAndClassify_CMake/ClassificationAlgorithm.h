#ifndef CLASSIFICATIONALGORITHM_H
#define CLASSIFICATIONALGORITHM_H

#include <vector>

using namespace std;

class ClassificationAlgorithm
{
public:
    
    virtual vector<double> predict(const vector<int> testObject) = 0;
    
protected: 
    
    void normalize(vector<double>& vec);
};
#endif
