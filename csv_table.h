#pragma once
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <iomanip>
#include "csv_nodes.h"

using namespace std;
typedef pair <string, string> Table_Pair;
class csv_table
{
private:
	vector<csv_nodes> _node_groups;
	map<string, string> _data;
	string* _rows;
	string* _cols;
	int _rows_count;
	int _cols_count;
	const int operators_count = 4;
	const char operators[4] = { '+', '-', '/', '*' };
	exception _csv_exception(string message, int row = -1, int col = -1);
	string _get_cel_address(int row, int col);

public:
	csv_table();
	csv_table(int columns_count, int rows_count);
	void Print();
	void Save(string file_path);
	void Load(string file_path);

	void Generate_Graph(int lenght, int max_width);
	static string* Split_Line(string line, int& size);

	int* Get_Rows();
	string* Get_Columns();
	int Get_RowsCount();
	int Get_ColumsCount();

	string Get_Data(int row, string column);
};

