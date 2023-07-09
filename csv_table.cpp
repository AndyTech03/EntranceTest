#include "csv_table.h"

void csv_table::_extend_graph(string nodeA, string nodeB, csv_nodes& group)
{
    // nodeA должен присутствовать в group
    if (group.Contains(nodeA) == false)
        throw "Узел отсутствует!";

    // nodeBИ должен отсутствовать в group
    // Используйте метод Connect!
    if (group.Contains(nodeB))
        throw "Уже записано!";

    // Если относится к другому графу - объединяем
    // А второй граф удаляется
    for (vector<csv_nodes>::iterator iter = _node_groups.begin(); iter != _node_groups.end(); iter++)
        if (iter->Contains(nodeB))
        {
            group.Union(nodeA, nodeB, *iter);
            _node_groups.erase(iter);
            cout << "Объединение графов!" << endl;
            return;
        }

    // Иначе просто добавляем новый узел
    group.Add(nodeA, nodeB);
}

void csv_table::_generation_step(int depth, const int max_depth, const int max_children, unordered_map<string, string> allowed, csv_nodes& group)
{
}

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
{}

csv_table::csv_table(const int columns_count, const int rows_count)
{
    for (int row = 0; row < rows_count; row++)
        _rows.push_back(to_string(row + 1));

    const int letters_count = 26;

    for (int col = 0; col < columns_count; col++)
    {
        int num = col+1;
        string label;
        while (num > letters_count)
        {
            int index = num % letters_count;
            if (index == 0)
            {
                label = 'Z' + label;
                num = num / letters_count - 1;
            }
            else
            {
                label = (char)(index + 'A' -1) + label;
                num = num / letters_count;
            }
        }
        _cols.push_back((char)(num + 'A' -1) + label);
    }

    for (int row = 0; row < rows_count; row++)
        for (int col = 0; col < columns_count; col++)
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
    const int rows_count = Get_RowsCount();
    const int cols_count = Get_ColumsCount();

    const int data_widht = 15;
    const int head_width = 4;
    cout << left << setw(head_width) << "";
    for (int col = 0; col < cols_count; col++)
        cout << setw(data_widht) << _cols[col];
    cout << endl;

    for (int row = 0; row < rows_count; row++)
    {
        cout << setw(head_width) << _rows[row];
        for (int col = 0; col < cols_count; col++)
            cout << setw(data_widht) << _data.at(_get_cel_address(row, col));
        cout << endl;
    }
            
}

void csv_table::Save(string file_path)
{
    const int rows_count = Get_RowsCount();
    const int cols_count = Get_ColumsCount();

    ofstream file(file_path);
    string head;
    for (int col = 0; col < cols_count; col++)
    {
        if (col < cols_count)
            head += ",";
        head += _cols[col];
    }
    file << head << endl;
    for (int row = 0; row < rows_count; row++)
    {
        file << _rows[row] << ',';
        for (int col = 0; col < cols_count; col++)
        {
            string address = _get_cel_address(row, col);
            file << _data.at(address);
            if (col < cols_count - 1)
                file << ',';
        }
        if (row < rows_count - 1)
            file << endl;
    }
}

void csv_table::Load(string file_path)
{
    string line;

    _rows.clear();
    _cols.clear();

    ifstream file(file_path);

    // Read table head
    getline(file, line);
    vector<string> head = Split_Line(line);
    _cols = vector<string>(head.begin() + 1, head.end());
    const int file_col_count = head.size();

    // read data rows
    _data.clear();
    int row_number = 1;
    for (; getline(file, line); row_number++)
    {
        vector<string> cells = Split_Line(line);
        if (cells.size() != file_col_count)
            throw _csv_exception("Число ячеек не совпадает!", row_number);

        _rows.push_back(cells[0]);
        for (int col = 1; col < file_col_count; col++)
        {
            _data.insert(Table_Pair(_get_cel_address(row_number - 1, col-1), cells[col]));
        }
    }
}

void csv_table::Generate_Graph(int max_depth, int max_children, bool avoid_others)
{
    /*
    unordered_map<string, string> allowed;
    if (avoid_others)
        for (csv_nodes group: _node_groups)
            for (string node : group.Get_Contained())
                allowed.insert()
    string start = _cols[rand() % _cols_count] + _rows[rand() % _rows_count];
    cout << "OK!" << endl;
    */
}

vector<string> csv_table::Split_Line(string line)
{
    const char sep = ',';
    vector<string> result;
    string label;
    for (char c : line)
    {
        if (c == sep)
        {
            result.push_back(label);
            label = "";
        }
        else
            label += c;
    }
    return result;
}

vector<int> csv_table::Get_Rows()
{
    vector<int> result;
    for (string row : _rows)
    {
        result.push_back(stoi(row));
    }
    return result;
}

vector<string> csv_table::Get_Columns()
{
    vector<string> result;
    for (string col : _cols)
    {
        result.push_back(col);
    }
    return result;
}

int csv_table::Get_RowsCount()
{
    return _rows.size();
}

int csv_table::Get_ColumsCount()
{
    return _cols.size();
}

string csv_table::Get_Data(int row, string column)
{
    return _data.at(column + to_string(row));
}
