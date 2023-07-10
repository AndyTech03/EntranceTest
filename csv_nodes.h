#pragma once
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <chrono>
#include <ctime>

#include "csv_node.h"

using namespace std;


typedef pair <string, csv_node> Nodes_Pair;
class csv_nodes
{
private:
	unordered_map<string, csv_node> _nodes;

public:
	int Size();
	void Clear();
	void Add(string node);
	void Add(string parent, string node);
	void Union(string self_node, string other_node, csv_nodes other);
	void Connect(string nodeA, string nodeB);

	bool Contains(string node);
	bool Child_To(string node, string parent);
	bool Parent_To(string node, string child);

	vector<string> Get_Children(string node);
	vector<string> Get_Parents(string node);
	vector<string> Get_Contained();

	unordered_map<string, double> _func_timeburn;
	unordered_map<string, int> _func_calls;
};

