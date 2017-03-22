#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <fstream>
#include <regex>
#include <stdlib.h>

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
    std::vector<std::vector<int> > objectMatrix;
    std::vector<std::string> names;

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

    //обработка
    for(unsigned i = 0; i < dirContents.size(); ++i)
    {
        std::vector<int> objectFeatures(18, 0);

        //открыть файлы и прочесть их содержимое
        //TODO: распараллелить чтение из двух файлов
        std::ifstream fAsm;
        fAsm.open(dirContents[i].path().string());
        if(!fAsm)
        {
            std::cerr << "Can't open " + dirContents[i].path().string() << std::endl;
            continue;
        }

        std::string str = "";
        while(fAsm >> str)
        {
            if (str == "mov") ++objectFeatures[0];
            if (str == "add") ++objectFeatures[1];
            if (str == "cmp") ++objectFeatures[2];
            if (str == "jmp") ++objectFeatures[3];
            if (str == "inc") ++objectFeatures[4];
            if (str == "neg") ++objectFeatures[5];
            if (str == "mul") ++objectFeatures[6];
            if (str == "imul") ++objectFeatures[7];
            if (str == "div") ++objectFeatures[8];
            if (str == "idiv") ++objectFeatures[9];
        }
        fAsm.close();

        std::string nameBytes = dirContents[i].path().string();
        nameBytes.erase(nameBytes.end() - 4, nameBytes.end());
        nameBytes += ".bytes";

        std::ifstream fBytes;
        fBytes.open(nameBytes);
        if(!fBytes)
        {
            std::cerr << "Can't open " + nameBytes << std::endl;
            continue;
        }

        while (fBytes >> str)
        {
            if (str == "00")
                ++objectFeatures[10];
            if (str == "FF")
                ++objectFeatures[11];
            if (str == "??")
                ++objectFeatures[12];
            if (str == "01")
                ++objectFeatures[13];
            if (str == "10")
                ++objectFeatures[14];
            if(str == "CC")
                ++objectFeatures[15];
            if(str == "C0")
                ++objectFeatures[16];
            if (str == "A8")
                ++objectFeatures[15];
            if (str == "0C")
                ++objectFeatures[16];
            if (str == "11")
                ++objectFeatures[17];
        }
        fBytes.close();

        //только для обучающей выборки: записать в конец вектора метку класса
        if(std::string(argv[1]) == "train")
        {
            std::string objectName = nameBytes;
            objectName.erase(objectName.end() - 6, objectName.end());
            objectName.erase(objectName.begin(), objectName.begin() + 6);
            objectFeatures.push_back(labels[objectName]);
        }

        objectMatrix.push_back(objectFeatures);
        std::cout << std::to_string(i + 1) + " entries processed." << std::endl;
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
            fOutput << names[i] << ",";
        }
        for (unsigned j = 0; j < objectMatrix[i].size(); ++j)
            fOutput << objectMatrix[i][j] << ",";
        fOutput << std::endl;
    }
    fOutput.close();

    return 0;
}
