#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <fstream>
#include <regex>
#include <stdlib.h>
#include <thread>

void countAsm(std::string name, std::vector<int> &vec)
{
    std::ifstream fAsm;
    fAsm.open(name);
    if(!fAsm)
    {
        std::cerr << "Can't open " + name << std::endl;
        return;
    }

    std::string str = "";
    while(fAsm >> str)
    {
        if (str == "mov") ++vec[0];
        if (str == "add") ++vec[1];
        if (str == "cmp") ++vec[2];
        if (str == "jmp") ++vec[3];
        if (str == "inc") ++vec[4];
        if (str == "neg") ++vec[5];
        if (str == "mul") ++vec[6];
        if (str == "imul") ++vec[7];
        if (str == "div") ++vec[8];
        if (str == "idiv") ++vec[9];
    }
    fAsm.close();
}

void countBytes(std::string name, std::vector<int> &vec)
{
    std::ifstream fBytes;
    fBytes.open(name);
    if(!fBytes)
    {
        std::cerr << "Can't open " + name << std::endl;
        return;
    }

    std::string str = "";
    while (fBytes >> str)
    {
        if (str == "00")
            ++vec[10];
        if (str == "FF")
            ++vec[11];
        if (str == "??")
            ++vec[12];
        if (str == "01")
            ++vec[13];
        if (str == "10")
            ++vec[14];
        if (str == "A8")
            ++vec[15];
        if (str == "0C")
            ++vec[16];
        if (str == "11")
            ++vec[17];
    }
    fBytes.close();
}

void processObject(std::string mode, std::string nameAsm, std::map<std::string, int> labels, unsigned i, std::vector<std::vector<int> > &objectMatrix)
{
    if(i >= objectMatrix.size())
    {
        return;
    }

    std::vector<int> objectFeatures(18, 0);

    std::string nameBytes = nameAsm;
    nameBytes.erase(nameBytes.end() - 4, nameBytes.end());
    nameBytes += ".bytes";

    std::thread thrAsm(countAsm, nameAsm, std::ref(objectFeatures));
    std::thread thrBytes(countBytes, nameBytes, std::ref(objectFeatures));

    thrAsm.join();
    thrBytes.join();

    //только для обучающей выборки: записать в конец вектора метку класса
    if(mode == "train")
    {
        std::string objectName = nameBytes;
        objectName.erase(objectName.end() - 6, objectName.end());
        objectName.erase(objectName.begin(), objectName.begin() + 6);
        objectFeatures.push_back(labels[objectName]);
    }

    objectMatrix[i] = objectFeatures;
}

void emptyFunc()
{
    return;
}

int main(int argc, char* argv[])
{
    std::string dir = "";
    if(argc < 2)
    {
        std::cerr << "Too few parameters" << std::endl;
        return -1;
    }
    else
    {
        if(std::string(argv[1]) == "train")
        {
            dir = "train";
        }
        else if(std::string(argv[1]) == "test")
        {
            dir = "test";
        }
        else
        {
            std::cerr << "Unexpected parameter." << std::endl;
            return -1;
        }
    }

    //извлечение меток классов из файла
    std::map<std::string, int> labels;
    if(std::string(argv[1]) == "train")
    {
        std::ifstream fLabels;
        fLabels.open("trainLabels.txt", std::ios::in);
        if(!fLabels)
        {
            std::cerr << "Can't open trainLabels.txt" << std::endl;
            return -1;
        }
        std::string tempStr = "";
        int tempNum;
        while(fLabels >> tempStr >> tempNum)
        {
            labels.insert(std::pair<std::string, int>(tempStr, tempNum));
        }
        fLabels.close();
    }

    //подготовка итератора
    if(!boost::filesystem::exists(dir) || !boost::filesystem::is_directory(dir))
    {
        std::cerr << "Wrong dir" << std::endl;
        return -1;
    }

    //поиск файлов с расширением .asm и занесение их дескрипторов в вектор
    std::vector<boost::filesystem::directory_entry> dirContents;
    std::regex filter(".*asm");
    boost::filesystem::directory_iterator end;

    for(boost::filesystem::directory_iterator i(dir); i!=end; ++i)
    {
        std::smatch what;
        if(!std::regex_match(i->path().string(), what, filter)) continue;
        dirContents.push_back(*i);
    }

    int blockSize = std::thread::hardware_concurrency();
    std::vector<std::vector<int> > objectMatrix(dirContents.size());

    //новая обработка
    int count = 0;
    for(unsigned i = 0; i < dirContents.size(); i += blockSize)
    {
        std::vector<std::thread> threads(blockSize);
        for(unsigned j = 0; j < threads.size(); ++j)
        {
            if(i + j < dirContents.size())
            {
                threads[j] = std::thread(processObject,
                                         std::string(argv[1]),
                                         dirContents[i + j].path().string(),
                                         labels,
                                         i + j,
                                         std::ref(objectMatrix));
            }
            else
            {
                threads[j] = std::thread(emptyFunc);
            }

        }
        for(unsigned j = 0; j < threads.size(); ++j)
        {
            threads[j].join();
        }
        count += blockSize;
        std::cout << count << " entries processed." << std::endl;
    }

    //вывод в файл
    std::ofstream fOutput;
    if(std::string(argv[1]) == "train")
    {
        fOutput.open("objectMatrix.txt", std::ios::out);
    }
    else
    {
        fOutput.open("objectMatrixTest.txt", std::ios::out);
    }
    if(!fOutput)
    {
        std::cerr << "Can't open output file" << std::endl;
        return -1;
    }
    for (unsigned i = 0; i < objectMatrix.size(); ++i)
    {
        if(std::string(argv[1]) == "test")
        {
            std::string name = dirContents[i].path().string();
            name.erase(name.end() - 4, name.end());
            name.erase(name.begin(), name.begin() + 5);
            fOutput << name << ",";
        }
        for (unsigned j = 0; j < objectMatrix[i].size(); ++j)
            fOutput << objectMatrix[i][j] << ",";
        fOutput << std::endl;
    }
    fOutput.close();

    return 0;
}
