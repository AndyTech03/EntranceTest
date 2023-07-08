#pragma once
#include <unordered_set>
#include <iostream>

using namespace std;
struct csv_node
{
	csv_node();
	csv_node(unordered_set<string> parents, unordered_set<string> children);

	bool Has_Child(string node);
	bool Has_Parent(string node);

	unordered_set<string> Parents;
	unordered_set<string> Children;
};

