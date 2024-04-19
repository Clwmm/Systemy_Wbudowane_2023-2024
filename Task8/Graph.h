// Mateusz Ka³wa

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <map>
#include <sstream>

struct GraphNode
{
	// Number of task
	unsigned short task_num;
	unsigned short condition = 0;

	// Pair of successor and data transfer
	std::vector<std::pair<GraphNode*, int>> successors;

	GraphNode(const unsigned short& task_num, const unsigned short& condition)
		: task_num(task_num), condition(condition) {}

	void display(std::map<unsigned short, std::string>& displayTasks)
	{
		std::string temp;
		temp = "T" + std::to_string(task_num) + " " + std::to_string(successors.size());
		for (auto x : successors)
		{
			if (x.first->condition != 0)
				temp += " " + std::to_string(x.first->task_num) + "w(W" + std::to_string(x.first->condition) + "," + std::to_string(x.second) + ")";
			else
				temp += " " + std::to_string(x.first->task_num) + "(" + std::to_string(x.second) + ")";
		}
			
		displayTasks[this->task_num] = temp;
	}
};

class Graph
{
private:
	using node_ptr = GraphNode*;
	node_ptr root = nullptr;
	size_t size = 0;
	std::map<unsigned short, std::string> displayTasks;

	node_ptr searchRecursive(node_ptr node, const unsigned short& task_num)
	{
		if (node == nullptr)
			return nullptr;
		if (node->task_num == task_num)
			return node;

		for (auto x : node->successors)
		{
			if (x.first != nullptr)
			{
				node_ptr temp = searchRecursive(x.first, task_num);
				if (temp != nullptr)
					return temp;
			}
		}
		return nullptr;
	}

	void display_(node_ptr node)
	{
		if (node != nullptr)
			node->display(displayTasks);
		else
			return;
		for (auto x : node->successors)
			this->display_(x.first);
	}

	void delete_(node_ptr node)
	{
		for (auto x : node->successors)
			this->delete_(x.first);
		node->successors.clear();

		delete node;
	}

public:

	~Graph()
	{
		this->clear();
	}

	void clear()
	{
		if (root != nullptr)
		{
			delete_(root);
			root = nullptr;
		}
	}

	void push(std::pair<GraphNode*, int> pair, const unsigned short& number_of_task_to_be__assigned_to)
	{
		if (size == 0)
		{
			root = pair.first;
			++size;
			return;
		}

		GraphNode* node = nullptr;
		node = this->search(number_of_task_to_be__assigned_to);
		if (node != nullptr)
		{
			node->successors.push_back(pair);
			++size;
		}
	}

	void push(GraphNode* node)
	{
		if (size == 0)
		{
			root = node;
			++size;
		}
	}

	size_t getSize() { return size; }

	node_ptr search(const unsigned short& task_num)
	{
		return this->searchRecursive(root, task_num);
	}

	void display(std::stringstream& ss)
	{
		displayTasks.clear();
		this->display_(root);

		for (auto x : displayTasks)
			ss << x.second << std::endl;
	}
};

#endif // !GRAPH_H