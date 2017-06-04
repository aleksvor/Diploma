#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include <boost/tokenizer.hpp>

#include "ClassificationAlgorithm.h"
#include "KNNClassifier.h"

using namespace boost;

int inputFromFile(std::string fileName, std::vector<std::vector<int> >& objects, std::vector<std::string>* testNames = NULL)
{
    std::ifstream file;
    
    file.open(fileName);
    if(!file)
    {
        std::cerr << "Can't open file " + fileName + "!" << std::endl;
        return -1;
    }
    
    std::string line = "";
    while(getline(file, line))
    {
        std::vector<int> object;
        
        //создадим токенайзер, разделящий строку на значения
        tokenizer<escaped_list_separator<char> > tk(
            line,
            escaped_list_separator<char>('\\', ',', '\"'));
        
        //и пройдёмся по полученной коллекции
        for(tokenizer<escaped_list_separator<char> >::iterator it(tk.begin()); it != tk.end(); ++it)
        {
            std::string temp = *it;
            int number = 0;
            
            try
            {
                if(!temp.empty())
                {
                    number = std::stoi(temp);
                    object.push_back(number);
                }
            }
            catch(...)
            {
                if(!temp.empty() && temp != "\r" && testNames)
                {
                    testNames->push_back(temp);
                }
            }
        }
        
        objects.push_back(object);
    }
    
    file.close();
    return 0;
}

int main()
{
    //открытие csv-файла и парсинг
    std::vector<std::vector<int> > trainObjects;
    int result = inputFromFile("objectMatrix.txt", trainObjects);
    if(result)
    {
        std::cerr << "Failed to read training file" << std::endl;
        return result;
    }
    
    //чтение файла с тестовыми объектами
    std::vector<std::vector<int> > testObjects;
    std::vector<std::string> testNames;
    result = inputFromFile("objectMatrixTest.txt", testObjects, &testNames);
    if(result)
    {
        std::cerr << "Failed to read test file" << std::endl;
        return result;
    }
    
    //использование классификатора
    ClassificationAlgorithm* classAlg = new KNNClassifier(trainObjects, 48);
    std::vector<std::vector<double> > predictions;
    for(int i = 0; i < testObjects.size(); ++i)
    {
        std::vector<double> temp = classAlg->predict(testObjects[i]);
        predictions.push_back(temp);
    }

    //вывод в файл
    std::ofstream outputFile;
    outputFile.open("SubmissionKNC.csv");
    outputFile << "Id,Prediction1,Prediction2,Prediction3,Prediction4,Prediction5,Prediction6,Prediction7,Prediction8,Prediction9\n";
    
    for(int i = 0; i < predictions.size(); ++i)
    {
        outputFile << testNames[i] << ",";
        for (int j = 0; j < predictions[i].size() - 1; ++j)
        {
            outputFile /**<< fixed << setprecision(6)*/ << predictions[i][j] << ",";
        }
        outputFile /**<< fixed << setprecision(6)*/ << predictions[i][predictions[i].size() - 1] << std::endl;
    }
    
    outputFile.close();
    
    delete classAlg;
    return 0;
}
