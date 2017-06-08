#include "ClassificationAlgorithm.h"

void ClassificationAlgorithm::normalize(vector<double>& vec)
{
    double min = vec[0];
    double max = vec[0];
    
    for(int i = 1; i < vec.size(); ++i)
    {
        if(vec[i] > max)
        {
            max = vec[i];
        }
        if(vec[i] < min)
        {
            min = vec[i];
        }
    }
    
    for(int i = 0; i < vec.size(); ++i)
    {
        vec[i] = (vec[i] - min) / (max - min);
    }
}
