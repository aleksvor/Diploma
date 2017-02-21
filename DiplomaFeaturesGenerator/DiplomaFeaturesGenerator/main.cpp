#include <fstream>
#include <io.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int HexToDec(char c)
{
	switch (c)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'A':
		return 10;
	case 'B':
		return 11;
	case 'C':
		return 12;
	case 'D':
		return 13;
	case 'E':
		return 14;
	case 'F':
		return 15;
	}
}

int index(string str)
{
	return HexToDec(str[0]) * 16 + HexToDec(str[1]);
}

int main()
{
	vector<vector<int> > objectMatrix;
	ofstream fOutput;
	fOutput.open("D:\\Microsoft Malware Classification Challenge\\objectMatrix.txt", ios::out);
	if (!fOutput)
	{
		return -1;
	}
	vector<string> names;
	//ввод меток классов из trainLabels в map
	ifstream fLabels;
	fLabels.open("E:\\Microsoft Malware Classification Challenge\\trainLabels.txt", ios::in);
	map<string, int> labels;
	for (int i = 0; i < 10868; ++i)
	{
		string str;
		int num;
		fLabels >> str >> num;
		pair<string, int> p(str, num);
		labels.insert(p);
	}
	//обработка файлов из обучающей выборки
	int count = 0;
	struct _finddata_t asm_file;
	intptr_t hFileAsm = _findfirst("E:\\Microsoft Malware Classification Challenge\\train\\*.asm", &asm_file);
	do
	{
		struct _finddata_t bytes_file;
		//поиск файла bytes с таким же именем
		string s = asm_file.name;
		s.erase(s.end() - 4, s.end());
		s = "E:\\Microsoft Malware Classification Challenge\\train\\" + s + ".bytes";
		char* st = new char[s.length() + 1];
		strcpy(st, s.c_str());
		intptr_t hFileBytes = _findfirst(st, &bytes_file);
		//открытие потоков
		ifstream fsAsm;
		s = "E:\\Microsoft Malware Classification Challenge\\train\\" + (string)asm_file.name;
		delete[] st;
		st = new char[s.length() + 1];
		strcpy(st, s.c_str());
		fsAsm.open(st, ios::in);
		ifstream fsBytes;
		s = "E:\\Microsoft Malware Classification Challenge\\train\\" + (string)bytes_file.name;
		delete[] st;
		st = new char[s.length() + 1];
		strcpy(st, s.c_str());
		fsBytes.open(st, ios::in);
		//генерация признаков
		string str;
		vector<int> objectFeatures(19, 0);

		while (fsBytes >> str)
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
			//	  if(str == "СС")
			//		++objectFeatures[15];
			//	  if(str == "С0")
			//        ++objectFeatures[16];
			if (str == "A8")
				++objectFeatures[15];
			if (str == "0C")
				++objectFeatures[16];
			if (str == "11")
				++objectFeatures[17];
		}

		while (fsAsm >> str)
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
		s = asm_file.name;
		s.erase(s.end() - 4, s.end());
		names.push_back(s);
		objectFeatures[objectFeatures.size() - 1] = labels[s];
		objectMatrix.push_back(objectFeatures);
		//закрытие файлов
		delete[] st;
		_findclose(hFileBytes);
		fsAsm.close();
		fsBytes.close();
		++count;
		system("cls");
		cout << count << " entries processed." << endl;
	} while (_findnext(hFileAsm, &asm_file) == 0);
	//вывод обучающей выборки в файл
	for (int i = 0; i < objectMatrix.size(); ++i)
	{
		//fOutput << names[i] << ",";
		for (int j = 0; j < objectMatrix[i].size(); ++j)
			fOutput << objectMatrix[i][j] << ",";
		fOutput << endl;
	}
	fOutput.close();
	fLabels.close();
	_findclose(hFileAsm);
	return 0;
}