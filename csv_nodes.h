#pragma once
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include "csv_node.h"

using namespace std;


typedef pair <string, csv_node> Nodes_Pair;
class csv_nodes
{
private:
	unordered_map<string, csv_node> _nodes;

public:
	void Clear();
	void Add(string node);
	void Add(string node, vector<string> parents, vector<string> children);

	bool Contains(string node);
	bool Child_To(string node, string parent);
	bool Parent_To(string node, string child);

	vector<string> Get_Children(string node);
	vector<string> Get_Parents(string node);
};

