#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <fstream>
#include <regex>
#include <stdlib.h>

int main()
{
    std::vector<std::vector<int> > objectMatrix;
    std::vector<std::string> names;

    //извлечение меток классов из файла
    std::ifstream fLabels;
    fLabels.open("trainLabels.txt", std::ios::in);
    if(!fLabels)
    {
        std::cout << "Can't open trainLabels.txt";
        return -1;
    }
    std::map<std::string, int> labels;
    for (int i = 0; i < 10868; ++i)
    {
        std::string str;
        int num;
        fLabels >> str >> num;
        std::pair<std::string, int> p(str, num);
        labels.insert(p);
    }
    fLabels.close();

    //подготовка итератора
    std::string dir = "train";
    if(!boost::filesystem::exists(dir) || !boost::filesystem::is_directory(dir))
    {
        std::cout << "Wrong dir";
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
            std::cout << "Can't open " + dirContents[i].path().string();
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
            std::cout << "Can't open " + nameBytes;
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
        std::string objectName = nameBytes;
        objectName.erase(objectName.end() - 6, objectName.end());
        objectName.erase(objectName.begin(), objectName.begin() + 6);
        objectFeatures.push_back(labels[objectName]);

        objectMatrix.push_back(objectFeatures);
        std::cout << std::to_string(i + 1) + " entries processed.\n";
    }

    //вывод в файл
    std::ofstream fOutput;
    fOutput.open("objectMatrix.txt", std::ios::out);
    if(!fOutput)
    {
        std::cout << "Can't open output file";
        return -1;
    }
    for (unsigned i = 0; i < objectMatrix.size(); ++i)
    {
        //fOutput << names[i] << ",";
        for (unsigned j = 0; j < objectMatrix[i].size(); ++j)
            fOutput << objectMatrix[i][j] << ",";
        fOutput << std::endl;
    }
    fOutput.close();

    return 0;
}
