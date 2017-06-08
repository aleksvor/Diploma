#include "KNNClassifier.h"

#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <iostream>

KNNClassifier::KNNClassifier(const std::vector<std::vector<int> > aMatrix, const int ak)
{
    if(aMatrix.size() <= 0)
    {
        throw std::length_error("Matrix is empty!");
    }
    
    trainObjects.resize(aMatrix.size());
    classLabels.resize(aMatrix.size());
    
    for(int i = 0; i < aMatrix.size(); ++i)
    {
        trainObjects[i].resize(aMatrix[i].size() - 1);
        
        for(int j = 0; j < aMatrix[i].size() - 1; ++j)
        {
            trainObjects[i][j] = (double)aMatrix[i][j];
        }
        
        normalize(trainObjects[i]);
        
        classLabels[i] = aMatrix[i][aMatrix[i].size() - 1];
    }
    
    k = ak;
}

double KNNClassifier::weight(const double range)
{
    return (1 / range);
}

std::vector<double> KNNClassifier::predict(const std::vector<int> testObject)
{
    std::vector<double> predictions(9, 0);
    
    std::vector<double> tempVec(testObject.size());
    for(int i = 0; i < testObject.size(); ++i)
    {
        tempVec[i] = testObject[i];
    }
    
    normalize(tempVec);
    
    //посчитаем расстояния до каждого объекта выборки
    std::vector<pair<int, double> > ranges(trainObjects.size());
    for(int i = 0; i < ranges.size(); ++i)
    {
        try
        {
            pair<int, double> temp(
                classLabels[i],
                calculateRange(tempVec, trainObjects[i]));
            ranges[i] = temp;
        }
        catch(std::length_error)
        {
            throw std::length_error("Vectors have different sizes!");
        }
    }
    
    //отсортируем объекты по расстоянию
    std::sort(ranges.begin(), ranges.end(), [](auto &left, auto &right) {
      return left.second < right.second;
    });
    
    //выкидываем элементы, не входящие в k ближайших соседей
    ranges.erase(ranges.begin() + k, ranges.end());
    
    //вычислим ответы для каждого класса
    for(int i = 0; i < predictions.size(); ++i)
    {
        predictions[i] = 0;
        for(int j = 0; j < ranges.size(); ++j)
        {
            predictions[i] += (ranges[j].first == i + 1) * weight(ranges[j].second);
        }
    }
    
    normalize(predictions);
    
    return predictions;
}

double KNNClassifier::calculateRange(const vector<double> vec1, const vector<double> vec2)
{
    if(vec1.size() != vec2.size())
    {
        std::cerr << vec1.size() << " " << vec2.size() << std::endl;
        throw std::length_error("Vectors have different sizes!");
    }
    
    double res = 0;
    
    for(int i = 0; i < vec1.size(); ++i)
    {
        res += (vec1[i] - vec2[i]) * (vec1[i] - vec2[i]);
    }
    
    return std::sqrt(res);
}
