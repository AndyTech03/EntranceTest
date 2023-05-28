#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <regex>

using namespace std;
/*
	collumn - number, unsigned
	row - text, en -> converted to index
	cell - number or operation
		operation - =<colrow><+,-,*,/><colrow>
*/

const int SEGMENT_SIZE = 2;

const regex WALIDATION("=-?([a-zA-Z]+)?\\d+[+\\-*\\/]([a-zA-Z]+)?\\d+|=?-?\\d+|=-?[a-zA-Z]+\\d+");
const regex SEARCHING("=-?([a-zA-Z]+)?(\\d+)[+\\-*\\/]([a-zA-Z]+)?(\\d+)|=-?([a-zA-Z]+)(\\d+)");

void convert_from_csv(string line, int& count, string*& data)
{
	count = 0;
	for (char c : line)
	{
		if (c == ',')
			count++;
	}

	string* result = new string[++count];
	result[0] = "";
	int current = 0;
	for (char c : line)
	{
		if (c == ',')
		{
			if (++current > count)
				break;

			result[current] = "";
			continue;
		}
		result[current] += c;
	}
	if (data != NULL)
		delete[] data;
	data = result;
}

int find_row(map<int, unsigned int>& rows, int number)
{
	for (auto row : rows)
	{
		if (row.second == number)
			return row.first;
	}
	return -1;
}

bool read_line(string*& line, int& count, ifstream& source) {

	string raw;
	source >> raw;

	if (source.eof())
		return false;

	convert_from_csv(raw, count, line);

	return true;
}

void read_table_head(string*& columns, int& count, ifstream& source)
{
	string* headers = NULL;
	read_line(headers, count, source);
	if (columns != NULL)
		delete[] columns;
	columns = new string[count];
	for (int i = 1; i < count; i++)
		columns[i] = headers[i];
}

int contains(string* source, int count, string item)
{
	for (int i = 0; i < count; ++i) {
		if (source[i] == item) {
			return i;
		}
	}
	return -1;
}

// Read table, saving headers and validating cell content
// Return true if OK
bool Try_Read_Headers_And_Validate_Cells (
	ifstream& source, 
	string*& columns, int& col_count,
	map<int, unsigned int>& rows, int& row_count
)
{
	read_table_head(columns, col_count, source);
	row_count = 1;

	string* line = NULL;
	int count;
	try {
		while (read_line(line, count, source))
		{
			int row_number = stoi(line[0]);
			if (count != col_count)
				throw logic_error("Wrong length of line: " + to_string(row_number) + "[" + to_string(row_count) + "]" + "!");

			if (rows.find(row_number) != rows.end())
				throw logic_error("Row: " + to_string(row_number) + "[" + to_string(row_count) + "]" + " exist!");
			rows[row_number] = row_count;

			for (int i = 1; i < col_count; i++)
			{
				if (regex_match(line[i], WALIDATION) == false)
					throw logic_error("Cell '" + columns[i] + " " + to_string(row_number) + "' is not valid!");

				smatch match;
				if (regex_search(line[i], match, SEARCHING))
				{
					string column;
					if (column != "")
					{
						// first column
						column = match[1].str();
						if (contains(columns, col_count, column) == -1)
							throw logic_error("Column '" + column + "' not exist!");

						if (column == columns[i] && match[2].str() == (to_string(row_number)))
							throw logic_error("Cell '" + columns[i] + " " + to_string(row_number) + "' is releted to itself!");

						// second column
						column = match[3].str();
						if (contains(columns, col_count, column) == -1)
							throw logic_error("Column '" + column + "' not exist!");

						if (column == columns[i] && match[4].str() == (to_string(row_number)))
							throw logic_error("Cell '" + columns[i] + " " + to_string(row_number) + "' is releted to itself!");
					}
					else
					{
						// alt column
						column = match[5].str();
						if (contains(columns, col_count, column) == -1)
							throw logic_error("Column '" + column + "' not exist!");

						if (column == columns[i] && match[6].str() == (to_string(row_number)))
							throw logic_error("Cell '" + columns[i] + " " + to_string(row_number) + "' is releted to itself!");
					}
				}
			}
			row_count++;
		}
		source.close();
		return true;
	}
	catch (exception e)
	{
		cout << e.what();
		source.close();
		return false;
	}
}

// Split all data in some temp-files using SEGMENTS_SIZE
void Package_Data(string filename, int row_count)
{
	ifstream source(filename);

	source.ignore('\n');
	ofstream temp_segment("temp_0.csv");
	for (int i = 0; i < row_count; i++)
	{
		if (i != 0 && i % SEGMENT_SIZE == 0)
		{
			temp_segment.close();
			temp_segment.open("temp_" + to_string(i / SEGMENT_SIZE) + ".csv");
		}
		string line;
		source >> line;
		temp_segment << line << endl;
	}
	if (temp_segment.is_open())
		temp_segment.close();

	source.close();
}

int get_group(vector<vector<int>>& groups, int row_index)
{
	int counter = 0;
	for (vector<int> items : groups)
	{
		for (int item : items)
			if (item == row_index)
				return counter;
		counter++;
	}
	return -1;
}

bool Try_Calculate(
	int row_index,
	map<int, unsigned int>& rows, const int row_count,
	string*& cols, const int col_count,
	vector<vector<int>>& groups
);

bool Try_Calculate_Next_Row(
	int next_row, int& group_index,
	map<int, unsigned int>& rows, int row_count,
	string*& cols, int col_count,
	vector<vector<int>>& groups
)
{
	if (rows.find(next_row) == rows.end())
	{
		cout << "Row '" + to_string(next_row) + "' not exist!";
		return false;
	}
	next_row = rows[next_row];

	// If next row not calculated yet, do it
	int next_row_group = get_group(groups, next_row);
	if (next_row_group == -1)
	{
		Try_Calculate(next_row, rows, row_count, cols, col_count, groups);
		next_row_group = get_group(groups, next_row);
	}

	// If it one group -> skip
	if (next_row_group == group_index)
		return true;

	if (next_row_group > group_index)
	{
		int temp = next_row_group;
		next_row_group = group_index;
		group_index = temp;
	}

	// Then -> union them
	vector<int>& current_group = groups[group_index];
	vector<int>& next_group = groups[next_row_group];
	for (int item : current_group)
		// ignore repiats
		if (find(next_group.begin(), next_group.end(), item) == next_group.end())
			next_group.push_back(item);

	// Finaly delete previous group and fix index
	groups.erase(next(groups.begin(), group_index));
	group_index = next_row_group;

	return true;
}


// Don`t call with calculated cells!
bool Try_Calculate (
	const int row_index,
	map<int, unsigned int>& rows, const int row_count,
	string*& cols, const int col_count,
	vector<vector<int>>& groups
)
{
	// If just in any group -> throw exception!
	if (get_group(groups, row_index) != -1)
	{
		throw logic_error("Row " + to_string(find_row(rows, row_index)) + " just calculated!");
	}

	// Push back first, than may be merge
	groups.push_back(vector<int>{ row_index });
	int group_index = groups.size() - 1;

	string* line = NULL;
	int segment = (row_index - 1) / SEGMENT_SIZE;
	ifstream source("temp_" + to_string(segment) + ".csv");
	for (int i = 0; i < (row_index - 1) % SEGMENT_SIZE; i++)
		source.ignore(numeric_limits<streamsize>::max(), '\n');
	int _;
	read_line(line, _, source);
	source.close();

	regex regex;
	smatch match;
	for (int cell_index = 1; cell_index < col_count; cell_index++)
	{
		string cell = line[cell_index];
		regex = "=-?([a-zA-Z]+)?(\\d+)[+\\-*\\/]([a-zA-Z]+)?(\\d+)";
		if (regex_match(cell, match, regex))
		{
			string column1 = match[1];
			string column2 = match[3];
			if (column1 == "" && column2 == "")
				continue;

			if (column1 != "")
			{
				int next_row = stoi(match[2].str());
				if (Try_Calculate_Next_Row(next_row, group_index, rows, row_count, cols, col_count, groups) == false)
					return false;
			}

			if (column2 != "")
			{
				int next_row = stoi(match[4].str());
				if (Try_Calculate_Next_Row(next_row, group_index, rows, row_count, cols, col_count, groups) == false)
					return false;
			}
		}
		else
		{
			regex = "=-?([a-zA-Z]+)(\\d+)";
			if (regex_match(cell, match, regex) == false)
			{
				continue;
			}

			if (match[1].str() != "")
			{
				int next_row = stoi(match[2].str());
				if (Try_Calculate_Next_Row(next_row, group_index, rows, row_count, cols, col_count, groups) == false)
					return false;
			}
		}
	}
	return true;
}


int main(int argc, char** argv)
{
	if (argc != 2)
	{
		cout << "CSV filename not seted!" << endl;
		return 1;
	}

	ifstream source(argv[1]);
	if (source.is_open() == false)
	{
		cout << "Can`t open the file!" << endl;
		return 1;
	}

	string* columns = NULL;
	int col_count;

	map<int, unsigned int> rows;
	int row_count;

	if (Try_Read_Headers_And_Validate_Cells(source, columns, col_count, rows, row_count) == false)
		return 1;

	Package_Data(argv[1], row_count);

	vector<vector<int>> groups;
	for (int row = 1; row < row_count; row++)
	{
		if (get_group(groups, row) != -1)
			continue;

		if (Try_Calculate(row, rows, row_count, columns, col_count, groups) == false)
		{
			return 1;
		}
	}

	for (vector<int> group : groups)
	{
		for (int item : group)
			cout << item << '\t';
		cout << endl;
	}
}
