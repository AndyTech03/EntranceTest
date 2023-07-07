#pragma once
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <random>

using namespace std;
typedef pair <string, string> string_Pair;
class csv_table
{
private:
	map<string, string> _data;
	string* _rows;
	string* _cols;
	int _rows_count;
	int _cols_count;

public:
	csv_table();
	csv_table(int columns_count, int rows_count);
	void Save(string file_path);

	int* Get_Rows();
	string* Get_Columns();
	int Get_RowsCount();
	int Get_ColumsCount();

	string Get_Data(int row, string column);
};

