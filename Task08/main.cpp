// Mateusz Ka³wa

#include <iostream>
#include <random>
#include <fstream>
#include "Graph.h"

#define MAX_TASK_NUMBER 65535

#define MAX_DATA_TRANSFER 250

#define MIN_PP_COST 55
#define MAX_PP_COST 250

#define MIN_HC_TIME 1
#define MAX_HC_TIME 70
#define MIN_PP_TIME 125
#define MAX_PP_TIME 970

#define MIN_HC_COST 450
#define MAX_HC_COST 980
#define MIN_PP_COST 15
#define MAX_PP_COST 100

#define MIN_COMM_COST 10
#define MAX_COMM_COST 90
#define MIN_COMM_B 1
#define MAX_COMM_B 25

Graph tasksGraphGenerator(const bool& only_zeros, const bool& conditions, const size_t& no_tasks)
{
	if (no_tasks < 1)
	{
		std::cout << "Number of tasks cannot be less than 1" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (no_tasks > MAX_TASK_NUMBER)
	{
		std::cout << "Number of tasks cannot be higher than " << MAX_TASK_NUMBER << std::endl;
		exit(EXIT_FAILURE);
	}
	Graph graph;
	graph.push(new GraphNode(0, 0));

	size_t condition_number = 1;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dataTransfer(1, MAX_DATA_TRANSFER);
	std::uniform_int_distribution<int> generateCondition(0, 3);

	for (size_t i = 1; i < no_tasks; i++)
	{
		std::uniform_int_distribution<int> taskNumber(0, i - 1);

		GraphNode* tempNode;

		if (conditions)
		{
			if (generateCondition(gen) == 0 && i != 1)
				tempNode = new GraphNode(i, condition_number++);
			else
				tempNode = new GraphNode(i, 0);
		}
		else
			tempNode = new GraphNode(i, 0);
			

		if (only_zeros)
			graph.push(std::pair<GraphNode*, int>(tempNode, 0), taskNumber(gen));
		else
			graph.push(std::pair<GraphNode*, int>(tempNode, dataTransfer(gen)), taskNumber(gen));
	}

	return graph;
}

std::vector<std::vector<int>> generatePE(const unsigned short& no_HC, const unsigned short& no_PP)
{
	if (no_HC + no_PP < 1)
	{
		std::cout << "Sum of Processing elements cannot be less than 1" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::vector<std::vector<int>> vec;
	for (size_t i = 0; i < no_HC; i++)
		vec.push_back(std::vector<int>({ 0, 0, 0 }));

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> costOfPP(MIN_PP_COST, MAX_PP_COST);

	for (size_t i = 0; i < no_PP; i++)
		vec.push_back(std::vector<int>({ costOfPP(gen), 0, 1 }));

	return vec;
}

std::vector<std::vector<int>> generateTimes(Graph& tasks, std::vector<std::vector<int>>& proc)
{
	std::vector<std::vector<int>> vec;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> timeHC(MIN_HC_TIME, MAX_HC_TIME);
	std::uniform_int_distribution<int> timePP(MIN_PP_TIME, MAX_PP_TIME);

	size_t no_tasks = tasks.getSize();
	for (size_t i = 0; i < no_tasks; i++)
	{
		std::vector<int> temp;
		for (auto x : proc)
			temp.push_back((x[2] == 1 ? timePP(gen) : timeHC(gen)));
		vec.push_back(temp);
	}

	return vec;
}

std::vector<std::vector<int>> generateCost(Graph& tasks, std::vector<std::vector<int>>& proc)
{
	std::vector<std::vector<int>> vec;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> costHC(MIN_HC_COST, MAX_HC_COST);
	std::uniform_int_distribution<int> costPP(MIN_PP_COST, MAX_PP_COST);

	size_t no_tasks = tasks.getSize();
	for (size_t i = 0; i < no_tasks; i++)
	{
		std::vector<int> temp;
		for (auto x : proc)
			temp.push_back((x[2] == 1 ? costPP(gen) : costHC(gen)));
		vec.push_back(temp);
	}

	return vec;
}

std::vector<std::vector<int>> generateComm(std::vector<std::vector<int>>& proc, const unsigned short& no_comms)
{
	std::vector<std::vector<int>> vec;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> cost(MIN_COMM_COST, MAX_COMM_COST);
	std::uniform_int_distribution<int> b(MIN_COMM_B, MAX_COMM_B);
	std::uniform_int_distribution<int> can_connect(0, 1);

	for (size_t i = 0; i < no_comms; i++)
	{
		std::vector<int> temp;
		temp.push_back(cost(gen));
		temp.push_back(b(gen));
		for (size_t i = 0; i < proc.size(); i++)
			temp.push_back(can_connect(gen));
		vec.push_back(temp);
	}

	// Corect min 2 connections to one comm
	for (size_t j = 0; j < vec.size(); ++j)
	{
		size_t sum = 0;
		for (size_t i = 2; i < vec[j].size(); i++)
			sum += vec[j][i];

		if (sum < 2)
		{
			size_t no_changes = 2 - sum;
			for (size_t i = 0; i < no_changes; i++)
			{
				for (size_t i = 2; i < vec[j].size(); i++)
				{
					if (vec[j][i] == 0)
					{
						vec[j][i] = 1;
						break;
					}
				}
			}
		}
	}

	// Corect min one connection to PE
	std::uniform_int_distribution<int> correct(0, vec.size() - 1);

	std::vector<int> sum;
	sum.resize(vec[0].size());
	for (size_t j = 0; j < vec.size(); ++j)
	{
		for (size_t i = 0; i < vec[j].size(); i++)
		{
			sum[i] += vec[j][i];
		}
	}

	for (size_t i = 2; i < sum.size(); i++)
		if (sum[i] < 1)
			vec[correct(gen)][i] = 1;

	return vec;
}

void displayTasks(Graph& tasks, std::stringstream& ss)
{
	ss << "@tasks " << tasks.getSize() << std::endl;
	tasks.display(ss);
}

void displayProc(std::vector<std::vector<int>>& proc, std::stringstream& ss)
{
	ss << "@proc " << proc.size() << std::endl;
	for (auto x : proc)
	{
		for (auto y : x)
			ss << y << " ";
		ss << std::endl;
	}
}

void displayTimes(std::vector<std::vector<int>>& times, std::stringstream& ss)
{
	ss << "@times" << std::endl;
	for (auto x : times)
	{
		for (auto y : x)
			ss << y << " ";
		ss << std::endl;
	}
}

void displayCost(std::vector<std::vector<int>>& cost, std::stringstream& ss)
{
	ss << "@cost" << std::endl;
	for (auto x : cost)
	{
		for (auto y : x)
			ss << y << " ";
		ss << std::endl;
	}
}

void displayComm(std::vector<std::vector<int>>& comm, std::stringstream& ss)
{
	ss << "@comm " << comm.size() << std::endl;
	for (size_t i = 0; i < comm.size(); i++)
	{
		ss << "CHAN" << i << " ";
		for (auto x : comm[i])
			ss << x << " ";
		ss << std::endl;
	}
}

auto main() -> int
{
	std::stringstream ss;

	unsigned short no_tasks = 0;
	unsigned short only_zeros = 0;
	unsigned short conditions = 0;
	unsigned short no_HC = 0;
	unsigned short no_PP = 0;
	unsigned short no_Comms = 0;
	std::string outputFileName;

	std::cout << "Enter number of tasks: ";
	std::cin >> no_tasks;
	std::cout << "Enter 1 to set data transfer of tasks to 0 or enter 0 to random generate: ";
	std::cin >> only_zeros;
	std::cout << "Enter 1 to randomly generate conditions or 0 to not have them: ";
	std::cin >> conditions;
	std::cout << "Enter number of HC: ";
	std::cin >> no_HC;
	std::cout << "Enter number of PP: ";
	std::cin >> no_PP;
	std::cout << "Enter number of CL: ";
	std::cin >> no_Comms;
	std::cout << "Enter name of output file: ";
	std::cin >> outputFileName;
	std::cout << std::endl;

	// user input
	auto tasks = tasksGraphGenerator(only_zeros, conditions, no_tasks);
	// user input
	auto proc = generatePE(no_HC, no_PP);

	auto times = generateTimes(tasks, proc);
	auto cost = generateCost(tasks, proc);

	// user input
	auto comm = generateComm(proc, no_Comms);

	displayTasks(tasks, ss);
	displayProc(proc, ss);
	displayTimes(times, ss);
	displayCost(cost, ss);
	displayComm(comm, ss);

	// user input
	std::ofstream outputFile(outputFileName);
	if (!outputFile.is_open())
	{
		std::cout << "Error: Unable to open file!" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string output = ss.str();

	outputFile << output;

	std::cout << output;
}