#include "csv_table.h"

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


    const int operators_count = 4;
    const char operators[operators_count] = { '+', '-', '/', '*' };

    for (int row = 0; row < _rows_count; row++)
        for (int col = 0; col < _cols_count; col++)
        {
            string address = _cols[col] + _rows[row];
            int state = rand() % 3;
            string data = "=";
            switch (state)
            {
            case 1:
            {
                char op = operators[rand() % operators_count];
                data += _cols[rand() % _cols_count] + _rows[rand() % _rows_count] + op;
            }
            case 2:
                data += _cols[rand() % _cols_count] + _rows[rand() % _rows_count];
                _data.insert(string_Pair(address, data));
                break;

            default:
                _data.insert(string_Pair(address, to_string(rand() % 1000)));
            }
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
            string address = _cols[col] + _rows[row];
            file << _data.at(address);
            if (col < _cols_count - 1)
                file << ',';
        }
        if (row < _rows_count - 1)
            file << endl;
    }
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
