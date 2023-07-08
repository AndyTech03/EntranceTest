#include <iostream>
#include <fstream>
#include <map>
#include "csv_table.h"

using namespace std;

const char HELP_CMD[] = "help";
const char READ_CMD[] = "read";
const char GENERATE_CMD[] = "generate";

void help();
void generate(string file_path, int row_count, int col_count);

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru");
	for (int i = 0; i < argc; i++)
		cout << argv[i] << endl;

	if (argc == 1)
	{
		cout << "Недостаточно аргументов!" << endl;
		help();
		return 1;
	}

	if (strcmp(argv[1], HELP_CMD) == 0)
	{
		help();
		return 0;
	}


	if (strcmp(argv[1], GENERATE_CMD) == 0)
	{
		if (argc != 5)
		{
			cout << "Неверное число аргументов!";
			return 1;
		}
		int row_count;
		int col_count;
		bool not_valid = sscanf_s(argv[3], "%d", &row_count) == sscanf_s(argv[4], "%d", &col_count) == false;

		if (not_valid)
		{
			cout << "Неверный формат размеров!";
			return 1;
		}

		if (row_count == 0 || col_count == 0)
		{
			cout << "Размеры должны быть отличными от 0!";
			return 1;
		}

		generate(argv[2], row_count, col_count);
		return 0;
	}

	cout << "Неизвествная команда " << argv[1];
	return 1;
}

void help()
{
	cout << "Программа для обработки csv файлов" << endl;
	cout << "Варианты запуска:" << endl;
	cout << "\t" << HELP_CMD << " - для получения этого сообщения" << endl;
	cout << "\t" << READ_CMD << " <имя файла>[.csv] - для открытия файла" << endl;
	cout << "\t" << GENERATE_CMD << " <имя файла>[.csv] <число строк> <число столбцов> - для генерации файла" << endl;
}

void generate(string file_path, int row_count, int col_count)
{
	csv_table data(col_count, row_count);
	/*
	data.Save(file_path);
	*/
	data.Load(file_path);
	data.Print();
}
