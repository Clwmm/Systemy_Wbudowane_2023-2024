// Mateusz Ka³wa

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <map>
#include <stack>
#include <sstream>
#include <set>
#include <fstream>

struct GraphNode
{
	// Number of task
	unsigned short task_num;

	// Pointer to parent
	std::vector<GraphNode*> parents;
	//GraphNode* parent = nullptr;

	// Pair of successor and data transfer
	std::vector<std::pair<GraphNode*, int>> successors;

	int time_start = -1;
	int time_end = -1;

	int proc_id = -1;
	int cost = -1;
	int proc_cost = -1;
	int time = -1;

	GraphNode(const unsigned short& task_num)
		: task_num(task_num) {}

	GraphNode(const int& task_num, const int& proc_id, const int& cost, const int& proc_cost, const int& time)
		: task_num(task_num), proc_id(proc_id), cost(cost), proc_cost(proc_cost), time(time) {}

	void display(std::map<unsigned short, std::string>& displayTasks)
	{
		std::string temp;
		temp = "T" + std::to_string(task_num) + " " + std::to_string(successors.size());
		for (auto x : successors)
			temp += " " + std::to_string(x.first->task_num) + "(" + std::to_string(x.second) + ")";
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

	// Matrixes
	std::vector<std::string> tasks;
	std::vector<std::vector<int>> proc;
	std::vector<std::vector<int>> times;
	std::vector<std::vector<int>> cost;
	std::vector<std::string> comm;

	int maximum_time = 0;

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

	void delete_(node_ptr node, node_ptr parent)
	{
		for (auto x : node->successors)
			this->delete_(x.first, node);
		node->successors.clear();

		for (auto x : node->parents)
			if (x != parent)
			{
				auto it = x->successors.begin();
				for (; it != x->successors.end(); ++it)
					if (it->first == node)
						break;
				if (it != x->successors.end())
					x->successors.erase(it);
			}

		delete node;
	}

	void removeTimeStartEnd_(node_ptr node, std::set<int>& set)
	{
		if (node != nullptr)
		{
			if (set.find(node->task_num) == set.end())
			{
				set.insert(node->task_num);
				node->time_start = -1;
				node->time_end = -1;
			}

			for (auto x : node->successors)
				this->removeTimeStartEnd_(x.first, set);
		}
	}

	void calculateGraph_(node_ptr node, int time_start)
	{
		if (node != nullptr)
		{
			if (node->time_start == -1)
			{
				node->time_start = time_start;
				node->time_end = node->time_start + node->time;
			}
			else if (time_start < node->time_start)
			{
				node->time_start = time_start;
				node->time_end = node->time_start + node->time;
			}
		}
		else
			return;
		for (auto x : node->successors)
			this->calculateGraph_(x.first, node->time_end);
	}

	void getAllocation_(node_ptr node, std::map<int, std::set<node_ptr>>& map)
	{
		if (node != nullptr)
			map[node->proc_id].insert(node);
		else
			return;
		for (auto x : node->successors)
			this->getAllocation_(x.first, map);
	}

	void displayIntervals_(node_ptr node, std::set<int>& set)
	{
		if (node != nullptr)
		{
			if (set.find(node->task_num) == set.end())
			{
				set.insert(node->task_num);
				std::cout << "T" << node->task_num << ": " << node->time_start << "-" << node->time_end << std::endl;
			}

			for (auto x : node->successors)
				this->displayIntervals_(x.first, set);
		}
	}

	void getSlowestTask_(node_ptr node, node_ptr& result)
	{
		if (result == nullptr)
			result = node;

		if (node->time_end > result->time_end)
			result = node;

		for (auto x : node->successors)
			getSlowestTask_(x.first, result);
	}

	void getFastestTask_(node_ptr node, node_ptr& result)
	{
		if (result == nullptr)
		{
			if (node->successors.size() == 0)
				result = node;
		}
		else
			if (node->successors.size() == 0 && node->time_end < result->time_end)
				result = node;

		for (auto x : node->successors)
			getFastestTask_(x.first, result);
	}

	int getFinalCost_(node_ptr node, std::set<int>& set)
	{
		int result = 0;
		if (node != nullptr)
		{
			if (set.find(node->task_num) == set.end())
			{
				set.insert(node->task_num);
				result += node->cost + node->proc_cost;
			}
			for (auto x : node->successors)
				result += this->getFinalCost_(x.first, set);
		}
		return result;
	}

	int getTime(const int& task_id, const int& proc_id)
	{
		return times[task_id][proc_id];
	}

	int getCost(const int& task_id, const int& proc_id)
	{
		return cost[task_id][proc_id];
	}

	int getCostProc(const int& proc_id)
	{
		return proc[proc_id][0];
	}

	int getMinTimeProc(const int& task_id)
	{
		int proc_id = -1;
		int min = INT_MAX;

		for (size_t i = 0; i < times[task_id].size(); i++)
		{
			if (times[task_id][i] < min)
			{
				min = times[task_id][i];
				proc_id = i;
			}
		}

		return proc_id;
	}

public:

	Graph(char* filename, int max_time)
		: maximum_time(max_time)
	{
		this->loadFromFile(filename);
	}

	~Graph()
	{
		this->clear();
	}

	void clear()
	{
		if (root != nullptr)
		{
			delete_(root, nullptr);
			root = nullptr;
		}
		this->size = 0;
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
			pair.first->parents.push_back(node);
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

	void displayIntervals()
	{
		std::set<int> set;
		this->displayIntervals_(root, set);
	}

	node_ptr getSlowestTask()
	{
		node_ptr result = nullptr;
		this->getSlowestTask_(root, result);
		return result;
	}

	node_ptr getFastestTask()
	{
		node_ptr result = nullptr;
		this->getFastestTask_(root, result);
		return result;
	}

	int getFinalTime()
	{
		return getSlowestTask()->time_end;
	}

	int getFinalCost()
	{
		std::set<int> set;
		return getFinalCost_(root, set);
	}

	std::vector<int> getCriticalPath()
	{
		std::vector<int> vec;
		node_ptr last = getSlowestTask();
		vec.push_back(last->task_num);

		while (last->parents.size() != 0)
		{
			for (auto x : last->parents)
			{
				if (x->time_end == last->time_start)
				{
					last = x;
					vec.push_back(last->task_num);
				}
			}
		}

		return vec;
	}

	void displayCriticalPath()
	{
		auto vec = getCriticalPath();

		for (int i = vec.size() - 1; i >= 0; i--)
		{
			if (i != 0)
				std::cout << "T" << vec[i] << " -> ";
			else
				std::cout << "T" << vec[i] << std::endl;
		}
	}

	void calculateGraph()
	{
		std::set<int> set;
		removeTimeStartEnd_(root, set);
		this->calculateGraph_(root, 0);
	}

	std::map<int, std::set<node_ptr>> getAllocation()
	{
		std::map<int, std::set<node_ptr>> map;
		this->getAllocation_(root, map);
		return map;
	}

	void displayAllocation()
	{
		auto allocation = this->getAllocation();
		for (auto x : allocation)
		{
			std::cout << "P" << x.first + 1 << ": ";
			for (auto y : x.second)
				std::cout << "T" << y->task_num << "(" << y->time_start << ") ";
			std::cout << std::endl;
		}
	}

	void setFastestResources()
	{
		for (size_t i = 0; i < this->getSize(); i++)
		{
			auto task = this->search(i);
			if (task != nullptr)
			{
				int proc_id_t = getMinTimeProc(i);
				int cost_t = getCost(i, proc_id_t);
				int time_t = getTime(i, proc_id_t);
				int cost_proc_t = getCostProc(proc_id_t);

				task->proc_id = proc_id_t;
				task->cost = cost_t;
				task->time = time_t;
				task->proc_cost = cost_proc_t;
			}
		}
		this->calculateGraph();
	}

	std::stack<std::pair<int, std::pair<int, int>>> getProcCostStack(const int& task_id)
	{
		// pair<proc_id, pair<time, cost>>
		std::stack<std::pair<int, std::pair<int, int>>> result;
		
		// pair<time, pair<proc_id, cost>>
		std::map<int, std::pair<int, int>> data;


		for (int i = 0; i < times[task_id].size(); ++i)
			data.emplace(times[task_id][i], std::make_pair(i, cost[task_id][i]));

		auto it = data.begin(); ++it;
		for (; it != data.end(); ++it)
			result.push(std::make_pair(it->second.first, std::make_pair(it->first, it->second.second)));

		/*for (auto x : data)
			result.push(std::make_pair(x.second.first, std::make_pair(x.first, x.second.second)));*/

		return result;
	}

	node_ptr getParent(node_ptr node)
	{
		if (node->parents.empty())
			return nullptr;

		for (auto x : node->parents)
			if (node->time_start == x->time_end)
				return x;

		return nullptr;
	}

	void refiningAlgorithm()
	{
		this->calculateGraph();
		int final_time = this->getFinalTime();
		if (final_time > this->maximum_time)
		{
			std::cout << "Time given too short for current resources." << std::endl;
			return;
		}

		node_ptr node = getFastestTask();
		// pair<proc_id, pair<time, cost>>
		std::stack<std::pair<int, std::pair<int, int>>> stack; // = getProcCostStack(node->task_num);
		std::vector<int> criticalPath = getCriticalPath();

		std::vector<int> tasks_to_realocate;
		for (int i = 0; i < this->getSize(); ++i)
			tasks_to_realocate.push_back(i);

		bool ended_fastest_path = false;

		while (true)
		{
			GraphNode copy_node(node->task_num, node->proc_id, node->cost, node->proc_cost, node->time);
			stack = getProcCostStack(node->task_num);

			bool choosedResource = false;
			
			auto it = std::find(tasks_to_realocate.begin(), tasks_to_realocate.end(), node->task_num);
			if (it != tasks_to_realocate.end())
				tasks_to_realocate.erase(it);

			while (!stack.empty())
			{
				auto top = stack.top(); stack.pop();

				node->proc_id = top.first;
				node->time = top.second.first;
				node->cost = top.second.second;
				node->proc_cost = getCostProc(node->proc_id);

				calculateGraph();

				final_time = this->getFinalTime();
				if (final_time > this->maximum_time)
					continue;
				else
				{
					choosedResource = true;
					break;
				}
			}
			if (!choosedResource)
			{
				node->proc_id = copy_node.proc_id;
				node->time = copy_node.time;
				node->cost = copy_node.cost;
				node->proc_cost = copy_node.proc_cost;
				calculateGraph();
			}

			if (!ended_fastest_path)
			{
				node_ptr parent = getParent(node);
				if (parent == nullptr)
					ended_fastest_path = true;
				else
					node = parent;
			}
			else
			{
				if (tasks_to_realocate.empty())
					break;
				node_ptr parent = this->search(tasks_to_realocate[0]);
				node = parent;
			}
		}
	}

	void loadFromFile(char* filename)
	{
		std::ifstream inputFile(filename);

		if (!inputFile)
		{
			std::cout << "Error openning file!\n";
			exit(EXIT_FAILURE);
		}

		std::string line;

		// @tasks
		std::getline(inputFile, line);
		std::string no_tasks_str = line.substr(7, line.size() - 7);
		int no_tasks = std::stoi(no_tasks_str);

		for (size_t i = 0; i < no_tasks; i++)
		{
			std::getline(inputFile, line);
			tasks.push_back(line);
		}

		// @proc
		std::getline(inputFile, line);
		std::string no_proc_str = line.substr(6, line.size() - 6);
		int no_proc = std::stoi(no_proc_str);

		
		for (size_t i = 0; i < no_proc; i++)
		{
			std::getline(inputFile, line);
			std::vector<int> row;
			std::istringstream iss(line);
			int value;

			while (iss >> value)
				row.push_back(value);

			proc.push_back(row);
		}

		// @times
		std::getline(inputFile, line);
		

		for (size_t i = 0; i < no_tasks; i++)
		{
			std::getline(inputFile, line);
			std::vector<int> row;
			std::istringstream iss(line);
			int value;

			while (iss >> value)
				row.push_back(value);

			times.push_back(row);
		}

		// @cost
		std::getline(inputFile, line);
		
		for (size_t i = 0; i < no_tasks; i++)
		{
			std::getline(inputFile, line);
			std::vector<int> row;
			std::istringstream iss(line);
			int value;

			while (iss >> value)
				row.push_back(value);

			cost.push_back(row);
		}

		// @comm
		std::getline(inputFile, line);
		std::string no_comm_str = line.substr(6, line.size() - 6);
		int no_comm = std::stoi(no_comm_str);
		
		for (size_t i = 0; i < no_comm; i++)
		{
			std::getline(inputFile, line);
			comm.push_back(line);
		}

		inputFile.close();

		// inserting tasks to graph
		for (size_t i = 0; i < tasks.size(); i++)
		{
			if (i == 0)
				this->push(new GraphNode(0));

			std::string x = tasks[i];
			std::istringstream iss(x);
			std::vector<std::string> words;

			std::string word;
			while (iss >> word)
				words.push_back(word);

			int n = std::stoi(words[1]);

			std::map<int, int> successors;

			for (size_t i = 2; i < words.size(); i++)
			{
				std::string y = words[i];

				int task_id = -1;
				int data = -1;
				size_t posOpen = y.find('(');
				size_t posClose = y.find(')');

				if (posOpen != std::string::npos)
					task_id = std::stoi(y.substr(0, posOpen));

				if (posOpen != std::string::npos && posClose != std::string::npos)
					data = std::stoi(y.substr(posOpen + 1, posClose - posOpen - 1));

				successors[task_id] = data;
			}

			for (auto x : successors)
			{
				auto node = this->search(x.first);
				if (node == nullptr)
					this->push(std::make_pair(new GraphNode(x.first), x.second), i);
				else
				{
					auto parent = this->search(i);
					if (parent != nullptr)
					{
						parent->successors.push_back(std::make_pair(node, x.second));
						node->parents.push_back(parent);
					}
				}
			}
		}
	}
};

#endif // !GRAPH_H