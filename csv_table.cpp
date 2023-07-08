#include "csv_table.h"

exception csv_table::_csv_exception(string message, int row, int col)
{
    if (row != -1)
        message += "\nСтрока: " + to_string(row);
    if (col != -1)
        message += "\nСтолбец: " + to_string(col);

    return exception(message.c_str());
}

string csv_table::_get_cel_address(int row, int col)
{
    return _cols[col] + _rows[row];
}

csv_table::csv_table()
{
    _rows = new string[0];
    _cols = new string[0];
    _rows_count = _cols_count = 0;
    _data = map<string, string>();
}

csv_table::csv_table(int columns_count, int rows_count)
{

    _rows_count = rows_count;
    _cols_count = columns_count;

    _rows = new string[_rows_count];
    _cols = new string[_cols_count];

    for (int row = 0; row < _rows_count; row++)
        _rows[row] = to_string(row + 1);

    const int letters_count = 26;

    for (int col = 0; col < _cols_count; col++)
    {
        int num = col+1;
        while (num > letters_count)
        {
            int index = num % letters_count;
            if (index == 0)
            {
                _cols[col] = 'Z' + _cols[col];
                num = num / letters_count - 1;
            }
            else
            {
                _cols[col] = (char)(index + 'A' -1) + _cols[col];
                num = num / letters_count;
            }
        }
        _cols[col] = (char)(num + 'A' -1) + _cols[col];
    }

    for (int row = 0; row < _rows_count; row++)
        for (int col = 0; col < _cols_count; col++)
        {
            string address = _get_cel_address(row, col);
            int state = rand() % 6;
            int num = rand() % 1000 - 500;
            int num1 = rand() % 1000 - 500;
            char op = operators[rand() % operators_count];
            switch (state)
            {
            case 1:
                _data.insert(Table_Pair(address, "=" + to_string(num % 20)));
                break;
            case 2:
                _data.insert(Table_Pair(address, to_string(num % 20)));
                break;
            case 3:
                _data.insert(Table_Pair(address, "=" + to_string(num % 20) + op + to_string(num1 % 20)));
                break;
            case 4:
                _data.insert(Table_Pair(address, "=" + to_string(num) + op + to_string(num1)));
                break;
            case 5:
                _data.insert(Table_Pair(address, "=" + to_string(num)));
                break;
            default:
                _data.insert(Table_Pair(address, to_string(num)));
            }
        }
}

void csv_table::Print()
{
    const int data_widht = 15;
    const int head_width = 4;
    cout << left << setw(head_width) << "";
    for (int col = 0; col < _cols_count; col++)
        cout << setw(data_widht) << _cols[col];
    cout << endl;

    for (int row = 0; row < _rows_count; row++)
    {
        cout << setw(head_width) << _rows[row];
        for (int col = 0; col < _cols_count; col++)
            cout << setw(data_widht) << _data.at(_get_cel_address(row, col));
        cout << endl;
    }
            
}

void csv_table::Save(string file_path)
{
    ofstream file(file_path);
    string head;
    for (int col = 0; col < _cols_count; col++)
    {
        if (col < _cols_count)
            head += ",";
        head += _cols[col];
    }
    file << head << endl;
    for (int row = 0; row < _rows_count; row++)
    {
        file << _rows[row] << ',';
        for (int col = 0; col < _cols_count; col++)
        {
            string address = _get_cel_address(row, col);
            file << _data.at(address);
            if (col < _cols_count - 1)
                file << ',';
        }
        if (row < _rows_count - 1)
            file << endl;
    }
}

void csv_table::Load(string file_path)
{
    string line;
    int size = 0;

    // Get file rows count
    ifstream file(file_path);
    delete[] _rows;
    while (file >> line)
        size++;
    _rows_count = size - 1;
    _rows = new string[_rows_count];
    size = 0;

    // Read table head and get table cols count
    file = ifstream(file_path);
    delete[] _cols;
    getline(file, line);
    string* head = Split_Line(line, size);
    _cols_count = size - 1;
    _cols = new string[_cols_count];
    // Copy head titles
    for (int i = 1; i < size; i++)
        _cols[i - 1] = head[i];
    delete[] head;

    // read data rows
    _data.clear();
    int row_number = 1;
    for (; getline(file, line); row_number++)
    {
        int s;
        string* cells = Split_Line(line, s);
        if (s != size)
            throw _csv_exception("Число ячеек не совпадает!", row_number);

        _rows[row_number - 1] = cells[0];
        for (int col = 1; col < size; col++)
        {
            _data.insert(Table_Pair(_get_cel_address(row_number - 1, col-1), cells[col]));
        }
    }
}

void csv_table::Generate_Graph(int lenght, int max_width)
{
    csv_nodes nodes;
    vector<string> parents;
    vector<string> children;

    nodes.Add("A1");

    parents.push_back("A1");
    nodes.Add("A2", parents, children);
    parents.clear();

    parents.push_back("A2");
    nodes.Add("A3", parents, children);
    parents.clear();

    parents.push_back("A1");
    children.push_back("A2");
    nodes.Add("B1", parents, children);
    children.clear();
    parents.clear();

    parents.push_back("B1");
    children.push_back("A3");
    nodes.Add("B2", parents, children);
    children.clear();
    parents.clear();

    for (string paretn : nodes.Get_Parents("A3"))
    cout << paretn << '\t';
    cout << endl;

    _node_groups.push_back(nodes);
}

string* csv_table::Split_Line(string line, int& size)
{
    const char sep = ',';
    size = count(line.begin(), line.end(), sep)+1;
    string* result = new string[size];
    int start = 0, end = 0;
    for (int i = 0; i < size; i++)
    {
        if (line[end] == sep)
        {
            end++;
            continue;
        }
        start = end;
        end = line.find(sep, start);
        result[i] = line.substr(start, end - start);
        end++;
    }
    return result;
}

int* csv_table::Get_Rows()
{
    int* result = new int[_rows_count];
    for (int i = 0; i < _rows_count; i++)
    {
        result[i] = stoi(_rows[i]);
    }
    return result;
}

string* csv_table::Get_Columns()
{
    string* result = new string[_cols_count];
    for (int i = 0; i < _cols_count; i++)
    {
        result[i] = _cols[i];
    }
    return result;
}

int csv_table::Get_RowsCount()
{
    return _rows_count;
}

int csv_table::Get_ColumsCount()
{
    return _cols_count;
}

string csv_table::Get_Data(int row, string column)
{
    return _data.at(column + to_string(row));
}
