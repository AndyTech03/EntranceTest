#pragma once
#include <iostream>

using namespace std;


struct csv_cell
{
	string Row;
	string Column;

	csv_cell(string code);
	csv_cell(int row, int column);
};

