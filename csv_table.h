#pragma once
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <iomanip>
#include <chrono>
#include <ctime>

#include "csv_nodes.h"

using namespace std;
typedef pair <string, string> Table_Pair;
typedef pair <string, vector<string>> Allowed_Pair;
class csv_table
{
private:
	char _sep;
	vector<csv_nodes> _node_groups;
	void _extend_graph(string nodeA, string nodeB, csv_nodes& group);

	unordered_map<string, double> _func_timeburn;
	unordered_map<string, int> _func_calls;

	int _generation_max_count;
	int _empty_cells_count;
	string _get_progress(csv_nodes& group);


	void _generation_step(
		int depth, const int max_depth,
		unordered_map<string, vector<string>> allowed, csv_nodes& group,
		string parent, bool other_avoiding, bool self_avoiding
	);
	string _get_random_allowed_node(unordered_map<string, vector<string>>& allowed, bool remove = true);
	void _remove_from_allowed(string col, int row_id, unordered_map<string, vector<string>>& allowed);

	map<string, string> _data;
	vector<string> _rows;
	vector<string> _cols;
	const int operators_count = 2;
	const char operators[2] = { '+', '-', /*'/', '*'*/ };
	exception _csv_exception(string message, int row = -1, int col = -1);
	string _get_cel_address(int row, int col);

public:
	csv_table();
	csv_table(const int columns_count, const int rows_count);
	void Print();
	void Save(string file_path);
	void Save(string file_path, const char sep);
	void Load(string file_path);

	void Generate_Graph(int max_depth, bool avoid_others=false, bool avoid_self=false);
	vector<string> Split_Line(string line);

	vector<int> Get_Rows();
	vector<string> Get_Columns();
	int Get_RowsCount();
	int Get_ColumsCount();

	string Get_Data(int row, string column);
};

