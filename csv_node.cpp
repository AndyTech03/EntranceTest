#include "csv_node.h"

csv_node::csv_node()
{}

csv_node::csv_node(unordered_set<string> parents, unordered_set<string> children)
{
	Parents = parents;
	Children = children;
}

bool csv_node::Has_Child(string node)
{
	return Children.find(node) != Children.end();
}

bool csv_node::Has_Parent(string node)
{
	return Parents.find(node) != Parents.end();
}
