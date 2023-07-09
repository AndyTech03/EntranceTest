#include "csv_nodes.h"


void csv_nodes::Clear()
{
	_nodes.clear();
}

void csv_nodes::Add(string node)
{
	if (_nodes.size() != 0)
		throw exception("Предназначен только для добавления первого элемента!");

	_nodes.insert(Nodes_Pair(node, csv_node()));
}

void csv_nodes::Add(string parent, string node)
{
	unordered_set<string> parents_set;
	unordered_set<string> children_set;

	if (Contains(node))
		throw exception("Уже записано!");

	if (_nodes.find(parent) == _nodes.end())
		throw exception("Родитель не найден!");

	// Соединение родителя и потомка
	if (parents_set.find(parent) == parents_set.end())
	{
		_nodes[parent].Children.insert(node);
		parents_set.insert(parent);
	}

	// Рекурсивное соединение с проотцами
	for (string grandpa : _nodes[parent].Parents)
	{
		if (grandpa == node)
			throw exception("Обнаружена рекурсия!");

		if (parents_set.find(grandpa) == parents_set.end())
		{
			_nodes[grandpa].Children.insert(node);
			parents_set.insert(grandpa);
		}
	}

	_nodes.insert(Nodes_Pair(node, csv_node(parents_set, children_set)));
}

// По завершению функции оидается удаление other_nodes, её поля не обновляются
void csv_nodes::Union(string self, string other, csv_nodes other_nodes)
{
	// Графы должны содержать свои пограничные элементы
	if (Contains(self) == false)
		throw "Элемент отсутвует!";
	if (other_nodes.Contains(other) == false)
		throw "Элемент отсутвует!";

	// Графы не должны содержать чужие пограничные элементы
	// Проверку наличия иных общих элементов опустим
	if (Contains(other))
		throw "Графы уже объедены! Удалите дубликат!";
	if (other_nodes.Contains(self))
		throw "Графы уже объедены! Удалите дубликат!";


	// Копируем узлы из other_nodes в текущий граф
	for (Nodes_Pair node : other_nodes._nodes)
	{
		_nodes.insert(node);
	}

	// Соединяем оба графа внутри текущего
	Connect(self, other);
}

void csv_nodes::Connect(string nodeA, string nodeB)
{
	// Оба узла должны принадлежать графу
	if (Contains(nodeA) == false)
		throw "Элемент отсутвует!";
	if (Contains(nodeB) == false)
		throw "Элемент отсутвует!";

	csv_node& node_a = _nodes[nodeA];
	csv_node& node_b = _nodes[nodeB];

	// Узлы не должны иметь циклической связи
	// Проверку наличия nodeB в родителях node_a пропустим
	if (node_b.Has_Child(nodeA))
		throw "Обноружена рекурсия!";

	vector<string> parents = Get_Parents(nodeA);
	vector<string> children = Get_Children(nodeB);


	// Добавляем node_b родителей node_a + nodeA
	node_b.Parents.insert(nodeA);
	for (string parent : parents)
		node_b.Parents.insert(parent);

	// Добавляем node_a потомков node_b + nodeB
	node_a.Children.insert(nodeB);
	for (string child : children)
		node_a.Children.insert(child);

	// Добавляет потомкам node_b родителей node_a + nodeA
	for (string child : children)
	{
		csv_node* child_node = &_nodes[child];

		child_node->Parents.insert(nodeA);
		for (string parent : parents)
			child_node->Parents.insert(parent);
	}

	// Добавляет родителям node_a потомков node_b + nodeB
	for (string parent : parents)
	{
		csv_node* parent_node = &_nodes[parent];

		parent_node->Children.insert(nodeB);
		for (string child : children)
			parent_node->Children.insert(child);
	}
}

bool csv_nodes::Contains(string node)
{
	return _nodes.find(node) != _nodes.end();
}

bool csv_nodes::Child_To(string node, string parent)
{
	if (Contains(node) == false)
		throw exception("Элемент отсутствует!");

	if (Contains(parent) == false)
		throw exception("Элемент отсутствует!");

	return _nodes[node].Has_Parent(parent);
}

bool csv_nodes::Parent_To(string node, string child)
{
	if (Contains(node) == false)
		throw exception("Элемент отсутствует!");

	if (Contains(child) == false)
		throw exception("Элемент отсутствует!");

	return _nodes[node].Has_Child(child);
}

vector<string> csv_nodes::Get_Children(string node)
{
	if (Contains(node) == false)
		throw exception("Элемент отсутствует!");

	vector<string> children;

	for (string child : _nodes[node].Children)
		children.push_back(child);

	return children;
}

vector<string> csv_nodes::Get_Parents(string node)
{
	if (Contains(node) == false)
		throw exception("Элемент отсутствует!");

	vector<string> parents;

	for (string parent : _nodes[node].Parents)
		parents.push_back(parent);

	return parents;
}

vector<string> csv_nodes::Get_Contained()
{
	vector<string> nodes;

	for (Nodes_Pair pair : _nodes)
		nodes.push_back(pair.first);

	return nodes;
}

