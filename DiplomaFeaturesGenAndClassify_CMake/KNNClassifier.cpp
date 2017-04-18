#include "KNNClassifier.h"

#include <stdexcept>
#include <cmath>
#include <algorithm>

KNNClassifier::KNNClassifier(const std::vector<std::vector<int> > aMatrix, const int ak)
{
    if(aMatrix.size() <= 0)
    {
        throw std::length_error("Matrix is empty!");
        return;
    }
    
    trainObjects = aMatrix;
    
    k = ak;
}

std::vector<double> KNNClassifier::predict(const std::vector<int> testObject)
{
    std::vector<double> predictions(trainObjects[0].size() - 1, 0); //-1 - т.к. последний элемент - метка класса
    
    //посчитаем расстояния до каждого объекта выборки
    std::vector<pair<int, double> > ranges(trainObjects.size());
    for(int i = 0; i < ranges.size(); ++i)
    {
        pair<int, double> temp(
            trainObjects[i][trainObjects[i].size() - 1],
            calculateRange(testObject, trainObjects[i]));
        ranges[i] = temp;
    }
    
    //отсортируем объекты по расстоянию
    std::sort(ranges.begin(), ranges.end(), [](auto &left, auto &right) {
      return left.second < right.second;
    });
    
    //выкидываем элементы, не входящие в k ближайших соседей
    ranges.erase(ranges.begin(), ranges.end() - k);
    
    //посчитаем количество элементов каждого класса
    std::vector<int> objectsAmount(trainObjects[0].size() - 1, 0);
    
    for(int i = 0; i < ranges.size(); ++i)
    {
        ++objectsAmount[ranges[i].first];
    }
    
    //посчитаем вероятности принадлежности каждому классу
    for(int i = 0; i < objectsAmount.size(); ++i)
    {
        predictions[i] = objectsAmount[i] / objectsAmount.size();
    }
    
    return predictions;
}

double KNNClassifier::calculateRange(const vector<int> vec1, const vector<int> vec2)
{
    if(vec1.size() != vec2.size())
    {
        throw std::length_error("Vectors have different sizes!");
        return -1;
    }
    
    double res = 0;
    
    for(int i = 0; i < vec1.size(); ++i)
    {
        res += std::pow((vec1[i] - vec2[i]), 2);
    }
    
    return std::sqrt(res);
}
